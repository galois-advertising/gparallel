// solopointer1202@gmail.com
#pragma once
#include <memory>
#include <sstream>
#include "executor.h"
#include "util.h"
namespace galois::gparallel {

inline void data_link_map_to_vec(const node_io_map & map, node_io_vec & vec) {
    for (auto && it : map) {
        vec.push_back(it.second);
    }
}
void operator += (node_io_vec & i, const node_io_vec & other);
node_io_vec operator + (node_io_vec & a, const node_io_vec & b);
typedef int id_t;
class node;
template<class meta_storage_t>
class node_schema;
template<class meta_storage_t>
using node_schema_ptr = std::shared_ptr<node_schema<meta_storage_t>>;
template<class meta_storage_t>
using node_schema_cptr = std::shared_ptr<const node_schema<meta_storage_t>>;

// This class holds all the information of a physical node, 
// including node name, node ID, upstream nodes, downstream nodes, 
// input and output metas.

template<class meta_storage_t>
class node_schema {
public:
    using node_schema_ptr_t = node_schema_ptr<meta_storage_t>;
    using executor_t = executable_t<meta_storage_t>;
    node_schema() {
        static int g_node_id = 0;
        _node_id = g_node_id++;
        _executor = nullptr;
        _deps_count = 0;
    }

    void initialize(std::string name, executor_t query_fn, const io_description & io) {
        _name = name;
        _executor = query_fn;

        auto fill_meta = [](const node_io_map & s, node_io_vec & d) {
            d.reserve(s.size());
            for (auto & node : s) {
                d.push_back(node.second);
            }
        };
        fill_meta(io.input, _input_metas);
        fill_meta(io.output, _output_metas);
    }

public: 
    bool delete_input_nodes(node_schema_ptr_t node) {
        return mutable_input_nodes().erase(node) > 0;
    }

public:
    // getter
    executor_t& mutable_executor() {
        return _executor;
    }

    node_io_vec& mutable_input_metas() {
        return _input_metas;

    }

    node_io_vec& mutable_output_metas() {
        return _output_metas;

    }
    const std::set<node_schema_ptr_t> & input_nodes() const {
        return _input_nodes;
    }

    std::set<node_schema_ptr_t> & mutable_input_nodes() {
        return _input_nodes;
    }

    const std::set<node_schema_ptr_t> & output_nodes() const {
        return _output_nodes;
    }

    std::set<node_schema_ptr_t> & mutable_output_nodes() {
        return _output_nodes;
    }

    std::string name() const {
        return _name;
    }

    int deps_count() const {
        return _deps_count;
    }

    int& mutable_deps_count() {
        return _deps_count;
    }

    int node_id() const {
        return _node_id;
    }

    void describe() const {
    }

    void graphviz(std::stringstream& out) const {
        for (auto node : _input_nodes) {
            out<<"\""<<name()<<"\" -> \""<<node->name()<<"\";"<<std::endl;
        }
    }
    executor_t _executor;
    id_t _node_id;
    int _deps_count;
    std::string _name;
    // All input and output metas
    node_io_vec _input_metas;
    node_io_vec _output_metas;
    // gParallel can infer upstream and downstream nodes 
    // through input and output meta which were staraged here
    std::set<node_schema_ptr_t> _input_nodes;
    std::set<node_schema_ptr_t> _output_nodes;
};
}