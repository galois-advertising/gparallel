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

void operator += (type_id_set_t & i, const type_id_set_t & other)
{
    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o;});
}
    //register_node_op<NT>();
    //for (auto & node : _nodes) {
    //    _name_node_map[node_ptr->name()] = _this_node;
    //    _name_options_map[node_ptr->name()] = options;
    //}

static node_info * const PRODUCE_MARK = reinterpret_cast<node_info*>(0x1);
static node_info * const SOUT_MARK = reinterpret_cast<node_info*>(0x2);

bool node_container::init()
{
    // check nodes
    if (auto pos = std::find_if(_nodes.begin(), _nodes.end(), [](auto ptr){
       return ptr->_query_out.size() > 0 && ptr->_item_out.size() > 0;
    }); pos != _nodes.end()) {
        //log(FATAL, "can not process both query and item:%s", (*pos)->name().c_str());
        return false;
    }

    // find all input<> for each output<> which depends them within this each node
    // for example: 
    //     nodeA: input<1:11> input<2> | out<3> out<4>
    //     nodeA: input<3> input<4:44> | out<5> out<6>
    // result is : 
    //     out<3> -> {input<1:11> input<11> input<2>}
    //     out<4> -> {input<1:11> input<11> input<2>}
    //     out<5> -> {input<3> input<4:44> input<44>}
    //     out<6> -> {input<3> input<4:44> input<44>}
    std::map<type_id_t, type_id_set_t> meta_implies;
    for (auto node : _nodes) {
        for (auto out : node->_data_out) {
            std::set<type_id_t> tmp;
            std::transform(node->_data_in.begin(), node->_data_in.end(),
                std::inserter(meta_implies[out.id], meta_implies[out.id].end()),
                [](auto & node){ return node.id; });
        }
    }

    // find all input<> in other node
    // for example: 
    // result is : 
    //     out<3> -> {input<1:11> input<11> input<2>}
    //     out<4> -> {input<1:11> input<11> input<2>}
    //     out<5> -> {input<3> input<4:44> input<44> input<1:11> input<11> input<2>}
    //     out<6> -> {input<3> input<4:44> input<44> input<1:11> input<11> input<2>}
    bool has_change = true;
    while (has_change) {
        has_change = false;
        // for every output
        for (auto & [output, input_set] : meta_implies) {
            //std::set<int> & s = it->second;
            type_id_set_t sub;
            // See every input<> as output<> of other nodes,
            // and we find the input<> or 'other nodes',
            // these input<> were also 
            for (auto input_id : input_set) {
                sub += meta_implies[input_id];
            }
            size_t prev_size = input_set.size();
            input_set += sub;
            if (input_set.size() > prev_size) {
                has_change = true;
            }
        }
    }
    std::stringstream meta_depends_log;
    meta_depends_log<<"\ndigraph gparallel_meta_depends {\n";
    meta_depends_log<<"rankdir=DT;\n";
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
    BOOST_LOG_TRIVIAL(warning) << "http://graphviz.it/#" <<std::endl;
    BOOST_LOG_TRIVIAL(warning) << std::regex_replace(log, re, "");

}
   


}