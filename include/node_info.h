#pragma once
#include <memory>
#include "util.h"
namespace galois::gparallel {
typedef int node_id_t;
class node;
class node_info;
typedef std::shared_ptr<node_info> node_ptr;
typedef std::shared_ptr<const node_info> node_cptr;
class node_info {
public:
    node_info();
    void initialize(std::string name, 
        batch_function_type batch_fn, query_function_type query_fn, 
        end_function_type end_fn, const io_description & io);
public:
    // setter
    void set_end(end_function_type end_fn);

public:
    // getter
    std::string name() const;
    int item_deps_count() const;
    int query_deps_count() const;
    size_t item_node_out_size() const;
    size_t query_node_out_size() const;
    const node_info & item_node_out(size_t index) const;
    const node_info & query_node_out(size_t index) const;
    int node_id() const;
    void describe() const;
    const char * graphviz() const;

//private:
    friend class node;
    friend class node_container;

    bool _has_item_input;
    batch_function_type _batch_fn;
    query_function_type _query_fn;
    end_function_type _end_fn;
    node_id_t _node_id;
    int _node_user_id;
    int _item_deps_count;
    int _query_deps_count;
    std::string _name;

    node_io_vec _item_input_meta;
    node_io_vec _item_output_meta;
    node_io_vec _query_input_meta;
    node_io_vec _query_output_meta;

    std::vector<node_ptr> _item_input_node;
    std::vector<node_ptr> _item_output_node;
    std::vector<node_ptr> _query_input_node;
    std::vector<node_ptr> _query_output_node;

    // just for	performance optimizing 
    node_io_vec _all_input_meta; // = _item_input_meta + _query_input_meta
    node_io_vec _all_output_meta; // = _item_output_meta + _query_output_meta
};
}