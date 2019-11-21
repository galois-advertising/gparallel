#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <boost/log/trivial.hpp>
#include "node_container.h"
#include "type_id.h"
#include "util.h"
#include <regex>


namespace galois::gparallel
{

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

static node_info * const PRODUCE_MARK = reinterpret_cast<node_info*>(0x1);
static node_info * const SOUT_MARK = reinterpret_cast<node_info*>(0x2);


void node_container::show_meta_depends_graphviz(
    id_implies_t & meta_implies,
    std::string tag)
{
    std::stringstream meta_depends_log;
    meta_depends_log<<"\ndigraph "<<tag<<"{\n";
    meta_depends_log<<"rankdir=BT;\n";
    meta_depends_log<<"size=\"8,5\";\n";
    for (auto & p : meta_implies) {
        std::cout<<typeid_manager<none_type>::instance().name(p.first)<<std::endl;;
        for (auto id : p.second) {
                meta_depends_log<<"\""<<typeid_manager<none_type>::instance().name(p.first)
                <<"\" -> \""<<typeid_manager<none_type>::instance().name(id)<<"\""<<";"<<std::endl;
        }
    }
    meta_depends_log<<"}";
    auto log = meta_depends_log.str();
    std::regex re("galois::gparallel::none_type, |galois::gparallel::meta_info_list");
    BOOST_LOG_TRIVIAL(warning) <<tag<<std::endl<<"http://graphviz.it/#" <<std::endl<<std::regex_replace(log, re, "");

}

void node_container::show_node_depends_graphviz(std::string tag)
{
    std::stringstream node_depends_log;
    node_depends_log<<"\ndigraph "<<tag<<"{\n";
    node_depends_log<<"rankdir=BT;\n";
    node_depends_log<<"size=\"8,5\";\n";
    for (auto node : _nodes) {
        node->graphviz(node_depends_log);
    }
    node_depends_log<<"}";
    auto log = node_depends_log.str();
    std::regex re("galois::gparallel::none_type, |galois::gparallel::meta_info_list");
    BOOST_LOG_TRIVIAL(warning) <<tag<<std::endl<<"http://graphviz.it/#" <<std::endl<<std::regex_replace(log, re, "");

}


bool node_container::graphviz(id_implies_t & target, std::stringstream & ss, std::string tag)
{
    ss<<"digraph "<<tag<<"{"<<std::endl;
    ss<<"rankdir=BT;"<<std::endl;
    ss<<"size=\"8,5\";"<<std::endl;
    for (auto & [output, input_set] : target) {
        for (auto input : input_set) {
                ss<<"\""<<output<<"\" -> \""<<input<<"\""<<";"<<std::endl;
        }
    }
    ss<<"}";
    return true;
}

bool node_container::make_full_connection(id_implies_t & implies)
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

bool node_container::build_meta_depends(id_implies_t & meta_implies)
{
    // find all input<> for each output<> which depends them within this each node
    // for example: 
    //     nodeA: input<1:11> input<2> | out<3> out<4>
    //     nodeA: input<3> input<4:44> | out<5> out<6>
    // result is : 
    //     out<3> -> {input<1:11> input<11> input<2>}
    //     out<4> -> {input<1:11> input<11> input<2>}
    //     out<5> -> {input<3> input<4:44> input<44>}
    //     out<6> -> {input<3> input<4:44> input<44>}
    for (auto node : _nodes) {
        for (auto i : {ITEM, QUERY}) {
            for (auto out : node->_output_metas[i]) {
                std::transform(node->_input_metas[i].begin(), node->_input_metas[i].end(),
                    std::inserter(meta_implies[out.id], meta_implies[out.id].end()),
                    [](auto & node){ return node.id; });
            }
        }
    }
    show_meta_depends_graphviz(meta_implies, "original_meta_implies");

    // find all input<> in other node
    // for example: 
    // result is : 
    //     out<3> -> {input<1:11> input<11> input<2>}
    //     out<4> -> {input<1:11> input<11> input<2>}
    //     out<5> -> {input<3> input<4:44> input<44> input<1:11> input<11> input<2>}
    //     out<6> -> {input<3> input<4:44> input<44> input<1:11> input<11> input<2>}
    if (make_full_connection(meta_implies)) {
        show_meta_depends_graphviz(meta_implies, "full_meta_implies");
    } else {
        BOOST_LOG_TRIVIAL(warning) << "meta: make_full_connection fail.";
        return false;
    } 
    return true;
}


bool node_in_vec(node_ptr node, std::vector<node_ptr> & vec) {
    return std::find(vec.begin(), vec.end(), node) != vec.end();
}

bool node_container::init()
{
    // check nodes
    if (auto pos = std::find_if(_nodes.begin(), _nodes.end(), [](auto ptr){
       return ptr->_output_metas[QUERY].size() > 0 && ptr->_output_metas[ITEM].size() > 0;
    }); pos != _nodes.end()) {
        //log(FATAL, "can not process both query and item:%s", (*pos)->name().c_str());
        return false;
    }
    meta_to_nodevec_t produce2nodevec;
    meta_to_nodevec_t sout2nodevec;
    meta_to_node_t output2node;
    for (auto node : _nodes) {
        for (auto i : {ITEM, QUERY}) {
            for (auto & meta: node->_output_metas[i]) {
                // for each output meta of each node
                switch (meta.type) {
                case OUTPUT:
                    if (output2node.count(meta.id) != 0) {
                        log(FATAL, "%s output fail: node[%s] already output meta[%s]",
                            node->name().c_str(), output2node[meta.id]->name().c_str(), 
                            typeid_manager<none_type>::instance().name(meta.id).c_str());
                        return false;
                    }
                    output2node[meta.id] = node;
                    break;
                case SOUT:
                    output2node[meta.id] = 
                        node_ptr(reinterpret_cast<node_info*>(SOUT_MARK), [](auto){});
                    sout2nodevec[meta.id].push_back(node);
                    break;
                case PRODUCE:
                    output2node[meta.id] = 
                        node_ptr(reinterpret_cast<node_info*>(PRODUCE_MARK), [](auto){});
                    produce2nodevec[meta.id].push_back(node);
                    break;
                case INPUT:
                case NONE:
                default:
                    log(FATAL, "should not be here");
                    break;
                }
            }
        }
    }


    id_implies_t meta_implies;
    build_meta_depends(meta_implies);


    for (auto node: _nodes) {
        id_set_t metas_direct_deps;
        {
            for (auto i : {ITEM, QUERY}) {
                metas_direct_deps += node->_input_metas[i];
            }
            bool has_reduced = true;
            while (has_reduced) {
                has_reduced = false;
                id_set_t tmp = metas_direct_deps;
                for (auto id : tmp) {
                    if (metas_direct_deps -= meta_implies[id]) {
                        has_reduced = true;
                        break;
                    } 
                }
            }
        }
        auto connect_node_by_meta = [&](int index) {
            for (auto & input_meta : node->_input_metas[index]) {
                // for each input meta of each node
                if (metas_direct_deps.find(input_meta.id) == metas_direct_deps.end()) {
                    // if this node direct depends this meta, then continue
                    continue;
                }

                auto upstream_node = output2node[input_meta.id];
                if (upstream_node.get() == PRODUCE_MARK) {
                    const auto & ups_nodes = produce2nodevec[input_meta.id];
                    if (ups_nodes.empty()) {
                        return false;
                    }

                    for (auto ups_node : ups_nodes) {
                        if (node_in_vec(ups_node, node->_input_nodes[index])) {
                            continue;
                        }
                        node->_input_nodes[index].push_back(ups_node);
                        ups_node->_output_nodes[index].push_back(node);
                    }
                    node->_deps_count[index] += 1;
                } else if (upstream_node.get() == SOUT_MARK) {
                    const auto & ups_nodes = sout2nodevec[input_meta.id];
                    if (ups_nodes.empty()) {
                        return false;
                    }
                    for (auto ups_node : ups_nodes) {
                        if (node_in_vec(ups_node, node->_input_nodes[index])) {
                            continue;
                        }
                        node->_input_nodes[index].push_back(ups_node);
                        node->_deps_count[index] += 1;
                        ups_node->_output_nodes[index].push_back(node);
                    }
                } else if (upstream_node != nullptr) {
                    if (node_in_vec(upstream_node, node->_input_nodes[index])) {
                        // if this node already in node->_input_nodes[ITEM]
                        continue;
                    }
                    node->_input_nodes[index].push_back(upstream_node);
                    node->_deps_count[index] += 1;
                    upstream_node->_output_nodes[index].push_back(node);
                } else {
                    return false;
                }
            }
            return true;
        };
        connect_node_by_meta(ITEM);
        connect_node_by_meta(QUERY);
    }
    show_node_depends_graphviz("node_depends");
    // reduce node deps
    id_implies_t node_implies;
    for (auto node : _nodes) {
        for (auto i : {ITEM, QUERY}) {
            for (const auto & node_out : node->_output_nodes[i]) {
                node_implies[node_out->node_id()].insert(node->node_id());
            }
        }
    }
    if (!make_full_connection(node_implies)) {
        BOOST_LOG_TRIVIAL(warning) << "node: make_full_connection fail.";
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
        //for (auto i : {ITEM, QUERY}) {
        //    auto input_node_itr = node->_input_nodes[i].begin();
        //    while (input_node_itr != node->_input_nodes[i].end()) {
        //        if (nodes_direct_deps.count((*input_node_itr)->node_id())) {
        //            continue;
        //        }
        //        auto & output_nodes = (*input_node_itr)->_output_nodes[i];
        //        auto it = std::find(output_nodes.begin(), output_nodes.end(), node);
        //        if (it == output_nodes.end()) {
        //            return false;
        //        } else {
        //            output_nodes.erase(it);
        //        }
        //        output_nodes.erase(*input_node_itr);
        //        node->_deps_count[i] -= 1;
        //        node->_input_nodes[i].erase(input_node_itr);
        //    }
        //}

    }
}  

}