// solopointer1202@gmail.com
#include <algorithm>
#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include "dag_schema.h"
#include "type_id.h"
#include "log.h"
#include "util.h"
#include "debug.h"


namespace galois::gparallel
{

typedef std::map< id_t, std::vector<node_schema_ptr> > meta_to_nodevec_t;
typedef std::map< id_t, node_schema_ptr > meta_to_node_t;

void operator += (id_set_t & i, const id_set_t & other)
{
    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o;});
}

bool operator -= (id_set_t & i, const id_set_t & other)
{
    id_set_t diff; 
    if (i.size() < other.size()) {
        for (auto id : i) {
            if (other.find(id) != other.end()) {
                diff.insert(id);
            }
        }
    } else {
        for (auto id : other) {
            if (i.find(id) != i.end()) {
                diff.insert(id);
            }
        }
    }
    for (auto id : diff) {
        i.erase(id);
    }
    return diff.size() != 0;

    
}

void operator += (id_set_t & i, const node_io_vec & other)
{
    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o.id;});
}

static node_schema * const PRODUCE_MARK = reinterpret_cast<node_schema*>(0x1);
static node_schema * const SOUT_MARK = reinterpret_cast<node_schema*>(0x2);

bool transitive_closure(topology_t & implies)
{
    bool has_change = true;
    while (has_change) {
        has_change = false;
        // for every output
        for (auto & [output, input_set] : implies) {
            id_set_t sub;
            // See every input<> as output<> of other nodes,
            // and we find the input<> or 'other nodes',
            // these input<> were also 
            for (auto input_id : input_set) {
                sub += implies[input_id];
            }
            size_t prev_size = input_set.size();
            input_set += sub;
            if (input_set.size() > prev_size) {
                has_change = true;
            }
        }
    }
    return true;

}

bool build_meta_topology(const dag_schema & _nodes, topology_t & me) {
    for (auto node : _nodes) {
        for (auto out : node->_output_metas) {
            std::transform(node->_input_metas.begin(), node->_input_metas.end(),
                std::inserter(me[out.id], me[out.id].end()),
                [](auto & node){ return node.id; });
        }
    }
    return true;
}

bool build_node_topology(const dag_schema & _nodes, topology_t & me) {
    for (auto node : _nodes) {
        for (const auto & node_out : node->_output_nodes) {
            me[node_out->node_id()].insert(node->node_id());
        }
    }
    return true;
}

bool if_node_in(node_schema_ptr node, std::set<node_schema_ptr> & vec) {
    return std::find(vec.begin(), vec.end(), node) != vec.end();
}

bool setup_dag_schema(dag_schema & _nodes)
{
    if (auto pos = std::find_if(_nodes.begin(), _nodes.end(), [](auto ptr){
        size_t query_meta_cnt = 0;
        size_t item_meta_cnt = 0;
        for (auto & output : ptr->_output_metas) {
            if (output.meta_level == meta_level_t::ITEM) {
                item_meta_cnt ++;
            } else if (output.meta_level == meta_level_t::QUERY) {
                query_meta_cnt ++;
            }
        }
        return item_meta_cnt > 0 && query_meta_cnt > 0;
    }); pos != _nodes.end()) {
        FATAL("can not process both query and item:%s", (*pos)->name().c_str());
        return false;
    }
    meta_to_nodevec_t produce2nodevec;
    meta_to_nodevec_t sout2nodevec;
    meta_to_node_t output2node;
    for (auto node : _nodes) {
        for (auto & meta: node->_output_metas) {
            // for each output meta of each node
            switch (meta.type) {
            case parameter_type::OUTPUT:
                if (output2node.count(meta.id) != 0) {
                    FATAL("%s output fail: node[%s] already output meta[%s]",
                        node->name().c_str(), output2node[meta.id]->name().c_str(), 
                        typeid_manager<none_type>::instance().name(meta.id).c_str());
                    return false;
                }
                output2node[meta.id] = node;
                break;
            case parameter_type::SOUT:
                output2node[meta.id] = 
                    node_schema_ptr(reinterpret_cast<node_schema*>(SOUT_MARK), [](auto){});
                sout2nodevec[meta.id].push_back(node);
                break;
            case parameter_type::PRODUCE:
                output2node[meta.id] = 
                    node_schema_ptr(reinterpret_cast<node_schema*>(PRODUCE_MARK), [](auto){});
                produce2nodevec[meta.id].push_back(node);
                break;
            case parameter_type::INPUT:
            case parameter_type::NONE:
            default:
                FATAL("should not be here", "");
                break;
            }
        }
    }


    topology_t meta_transitive_closure;
    if (build_meta_topology(_nodes, meta_transitive_closure)) {
        if (transitive_closure(meta_transitive_closure)) {
            debug::show_meta_depends_graphviz(meta_transitive_closure, "meta_transitive_closure");
        } else {
            FATAL("transitive_closure(meta_transitive_closure) fail", "");
            return false;
        } 
    }

    for (auto node : _nodes) {
        id_set_t metas_direct_deps;
        {
            metas_direct_deps += node->_input_metas;
            bool has_reduced = true;
            while (has_reduced) {
                has_reduced = false;
                id_set_t tmp = metas_direct_deps;
                for (auto id : tmp) {
                    if (metas_direct_deps -= meta_transitive_closure[id]) {
                        has_reduced = true;
                        break;
                    } 
                }
            }
        }
        auto link_node_by_meta = [&]() {
            for (auto & input_meta : node->_input_metas) {
                if (metas_direct_deps.find(input_meta.id) == metas_direct_deps.end()) {
                    // Only care about of the metas that direct depends.
                    continue;
                }

                auto upstream_node = output2node[input_meta.id];
                if (upstream_node.get() == PRODUCE_MARK) {
                    const auto & ups_nodes = produce2nodevec[input_meta.id];
                    if (ups_nodes.empty()) {
                        return false;
                    }

                    for (auto ups_node : ups_nodes) {
                        node->_input_nodes.insert(ups_node);
                        ups_node->_output_nodes.insert(node);
                    }
                    node->_deps_count += 1;
                } else if (upstream_node.get() == SOUT_MARK) {
                    const auto & ups_nodes = sout2nodevec[input_meta.id];
                    if (ups_nodes.empty()) {
                        return false;
                    }
                    for (auto ups_node : ups_nodes) {
                        if (if_node_in(ups_node, node->_input_nodes)) {
                            continue;
                        }
                        node->_input_nodes.insert(ups_node);
                        node->_deps_count += 1;
                        ups_node->_output_nodes.insert(node);
                    }
                } else if (upstream_node != nullptr) {
                    node->_input_nodes.insert(upstream_node);
                    upstream_node->_output_nodes.insert(node);
                    node->_deps_count += 1;
                } else {
                    return false;
                }
            }
            return true;
        };
        link_node_by_meta();
    }
    debug::show_node_depends_graphviz(_nodes, "node_depends");
    topology_t node_implies;
    build_node_topology(_nodes, node_implies);
    if (!transitive_closure(node_implies)) {
        FATAL("node: transitive_closure fail.", "");
        return false;
    }
    for (auto node : _nodes) {
        auto nodes_direct_deps = node_implies[node->node_id()];
        {
            bool has_reduced = true;
            while (has_reduced) {
                has_reduced = false;
                auto tmp = nodes_direct_deps;
                for (const auto& node_in : tmp) {
                    if (nodes_direct_deps -= node_implies[node_in]) {
                        has_reduced = true;
                        break;
                    }
                }
            }
        }
        for (auto input_node_itr = node->_input_nodes.begin(); 
            input_node_itr != node->_input_nodes.end(); ++input_node_itr) {
            if (nodes_direct_deps.count((*input_node_itr)->node_id())) {
                continue;
            }
            if ((*input_node_itr)->_output_nodes.erase(node) < 1) {
                return false;
            }
            node->_input_nodes.erase(input_node_itr);
            (*input_node_itr)->_output_nodes.erase(*input_node_itr);
            node->_deps_count -= 1;
        }
    }
    debug::show_node_depends_graphviz(_nodes, "node_depends_after");
    return true;
}  

}