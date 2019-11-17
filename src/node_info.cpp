#include <algorithm>
#include "util.h"
#include "node_info.h"


namespace galois::gparallel {

inline void data_link_map_to_vec(const node_io_map & map, node_io_vec & vec) {
    for (auto && it : map) {
        vec.push_back(it.second);
    }
}

void operator += (node_io_vec & i, const node_io_vec & other)
{
    i.insert(i.end(), other.begin(), other.end());
}

node_io_vec operator + (node_io_vec & a, const node_io_vec & b)
{
    node_io_vec res;
    res += a;
    res += b;
    return std::move(res);
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

    auto fill_meta = [](const node_io_map & s, node_io_vec & d) {
        d.reserve(s.size());
        for (auto & node : s) {
            d.push_back(node.second);
        }
    };
    fill_meta(io.item_input, _item_input_meta);
    fill_meta(io.item_output, _item_output_meta);
    fill_meta(io.query_input, _query_input_meta);
    fill_meta(io.query_output, _query_output_meta);

    _all_input_meta += _item_input_meta; 
    _all_input_meta += _query_input_meta;
    _all_output_meta += _item_output_meta;
    _all_output_meta += _query_output_meta;
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
    return _item_output_node.size();
}

const node_info& node_info::item_node_out(size_t index) const {
    return *_item_output_node[index];
}

int node_info::query_deps_count() const {
    return _query_deps_count;
}

size_t node_info::query_node_out_size() const {
    return _query_output_node.size();
}

const node_info& node_info::query_node_out(size_t index) const {
    return *_query_output_node[index];
}

int node_info::node_id() const {
    return _node_id;
}

}