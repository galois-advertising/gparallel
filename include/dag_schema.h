// solopointer1202@gmail.com
#pragma once
#include <string>
#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include <optional>
#include "node_deduce.h"
#include "node_schema.h"
#include "util.h"
#include "executor.h"
#include "type_id.h"
#include "debug.h"


namespace galois::gparallel {

template<class meta_storage_t>
using dag_schema = std::list<node_schema_ptr<meta_storage_t>>;

template<class meta_storage_t>
using meta_to_nodevec_t = std::map< id_t, std::vector<node_schema_ptr<meta_storage_t>> >;

template<class meta_storage_t>
using meta_to_node_t = std::map< id_t, node_schema_ptr<meta_storage_t>>;

template<class meta_storage_t>
std::optional<dag_schema<meta_storage_t>> topological_sort(const dag_schema<meta_storage_t>& const_dag) {
    auto dag = const_dag;
    dag_schema<meta_storage_t> result;
    while(!dag.empty()) {
        auto res = std::find_if(dag.begin(), dag.end(), [](auto node)->bool{
            return node->mutable_input_nodes().size() == 0;
        });
        if (res != dag.end()) {
            TRACE("Found 0-input node[%s]", (*res)->name().c_str());
            result.push_back(*res);
            auto del = *res;
            dag.erase(res);
            std::for_each(dag.begin(), dag.end(), [del](auto node){
                auto res = node->delete_input_nodes(del);
                TRACE("Delete input [%s] for [%s] : [%s]", 
                    del->name().c_str(), node->name().c_str(),
                    res ? "true" : "false");
            });
        } else {
            return std::nullopt;
        }
    }
    return result;
}

template <class NT, class meta_storage_t>
void register_node_operator(dag_schema<meta_storage_t> & c) {
    std::string name = demangle(typeid(NT).name());
    auto new_node = std::make_shared<node_schema<meta_storage_t>>();
    c.push_back(new_node);
    io_description vec;
    deduce_depends<NT>::deduce(vec);
    c.back()->initialize(name, nullptr, vec);
}

template <class meta_storage_t, class ...NTS>
struct register_node {
    static void reg(dag_schema<meta_storage_t> & c)
    {
        register_node<type_list<NTS ...>>::reg(c);
    };
};

template <class meta_storage_t, class NT, class ...NTS>
struct register_node<type_list<meta_storage_t, NT, NTS...>> {
    static void reg(dag_schema<meta_storage_t> & c)
    {
        register_node_operator<NT>(c);
        register_node<meta_storage_t, NTS...>::reg(c);
    };
};

template <class meta_storage_t>
struct register_node<meta_storage_t, type_list<>> {
    static void reg(dag_schema<meta_storage_t> & ) { /* stop recursion */}
};


void operator += (id_set_t & i, const id_set_t & other) {
    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o;});
}

bool operator -= (id_set_t & i, const id_set_t & other) {
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

void operator += (id_set_t & i, const node_io_vec & other) {
    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o.id;});
}


bool transitive_closure(topology_t& implies) {
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

template<class meta_storage_t>
bool build_meta_topology(const dag_schema<meta_storage_t> & _nodes, topology_t & me) {
    for (auto node : _nodes) {
        for (auto out : node->_output_metas) {
            std::transform(node->_input_metas.begin(), node->_input_metas.end(),
                std::inserter(me[out.id], me[out.id].end()),
                [](auto & node){ return node.id; });
        }
    }
    return true;
}

template<class meta_storage_t>
bool build_node_topology(const dag_schema<meta_storage_t> & _nodes, topology_t & me) {
    for (auto node : _nodes) {
        for (const auto & node_out : node->_output_nodes) {
            me[node_out->node_id()].insert(node->node_id());
        }
    }
    return true;
}

template<class meta_storage_t>
bool if_node_in(node_schema_ptr<meta_storage_t> node, std::set<node_schema_ptr<meta_storage_t>> & vec) {
    return std::find(vec.begin(), vec.end(), node) != vec.end();
}

template<class meta_storage_t>
bool setup_dag_schema(dag_schema<meta_storage_t> & _nodes) {
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
    meta_to_nodevec_t<meta_storage_t> produce2nodevec;
    meta_to_nodevec_t<meta_storage_t> sout2nodevec;
    meta_to_node_t<meta_storage_t> output2node;
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

                if (auto upstream_node = output2node[input_meta.id]; 
                    upstream_node != nullptr) {
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
    debug::show_node_depends_graphviz<meta_storage_t>(_nodes, "node_depends_after");
    return true;
}  


}
