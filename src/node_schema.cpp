#include <algorithm>
#include "util.h"
#include "node_schema.h"


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

node_schema::node_schema() {
    static int g_node_id = 0;
    _node_id = g_node_id++;
    _batch_fn = nullptr;
    _query_fn = nullptr;
    _end_fn = nullptr;
    _node_user_id = 0;
    _deps_count = 0;
}



void node_schema::initialize(
        std::string name, 
        batch_function_type batch_fn, 
        query_function_type query_fn, 
        end_function_type end_fn, 
        const io_description & io) {
    _name = name;
    _batch_fn = batch_fn;
    _query_fn = query_fn;
    _end_fn = end_fn;
    _has_item_input = (io.input.size() > 0);

    auto fill_meta = [](const node_io_map & s, node_io_vec & d) {
        d.reserve(s.size());
        for (auto & node : s) {
            d.push_back(node.second);
        }
    };
    fill_meta(io.input, _input_metas);
    fill_meta(io.output, _output_metas);
}

void node_schema::graphviz(std::stringstream & out) const {
    for (auto node : _input_nodes) {
        out<<"\""<<name()<<"\" -> \""<<node->name()<<"\";"<<std::endl;
    }
}

void node_schema::set_end(end_function_type end_fn) {
    _end_fn = end_fn;
}

void node_schema::describe() const {

}

std::string node_schema::name() const {
    return _name;
}

int node_schema::item_deps_count() const {
    return _deps_count;
}

size_t node_schema::item_node_out_size() const {
    return _output_nodes.size();
}

int node_schema::query_deps_count() const {
    return _deps_count;
}

size_t node_schema::query_node_out_size() const {
    return _output_nodes.size();
}

int node_schema::node_id() const {
    return _node_id;
}

}