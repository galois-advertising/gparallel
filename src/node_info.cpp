#include <algorithm>
#include "util.h"
#include "node_info.h"


namespace galois::gparallel {

inline void data_link_map_to_vec(const node_io_map & map, node_io_vec & vec) {
    for (auto && it : map) {
        vec.push_back(it.second);
    }
}


node_info::node_info() {
    static int g_node_id = 0;
    _node_id = g_node_id++;
    _batch_fn = nullptr;
    _query_fn = nullptr;
    _end_fn = nullptr;
    _node_user_id = 0;
    _item_deps_count = 0;
    _query_deps_count = 0;
}



void node_info::initialize(
        std::string name, 
        batch_function_type batch_fn, 
        query_function_type query_fn, 
        end_function_type end_fn, 
        const io_description & io) {
    _name = name;
    _batch_fn = batch_fn;
    _query_fn = query_fn;
    _end_fn = end_fn;
    _has_item_input = (io.item_input.size() > 0);

    auto fn = [](const node_io_map & s, node_io_vec & d) {
        d.reserve(s.size());
        for (auto & node : s) {
            d.push_back(node.second);
        }
    };
    fn(io.item_input, _item_in);
    fn(io.item_output, _item_out);
    fn(io.query_input, _query_in);
    fn(io.query_output, _query_out);

    _data_in.insert(_data_in.end(), _item_in.begin(), _item_in.end());
    _data_in.insert(_data_in.end(), _query_in.begin(), _query_in.end());
    _data_out.insert(_data_out.end(), _item_out.begin(), _item_out.end());
    _data_out.insert(_data_out.end(), _query_out.begin(), _query_out.end());
}

void node_info::set_end(end_function_type end_fn) {
    _end_fn = end_fn;
}

void node_info::describe() const {

}

const char* node_info::graphviz() const {

}

std::string node_info::name() const {
    return _name;
}

int node_info::item_deps_count() const {
    return _item_deps_count;
}

size_t node_info::item_node_out_size() const {
    return _item_node_out.size();
}

const node_info& node_info::item_node_out(size_t index) const {
    return *_item_node_out[index];
}

int node_info::query_deps_count() const {
    return _query_deps_count;
}

size_t node_info::query_node_out_size() const {
    return _query_node_out.size();
}

const node_info& node_info::query_node_out(size_t index) const {
    return *_query_node_out[index];
}

int node_info::node_id() const {
    return _node_id;
}

}