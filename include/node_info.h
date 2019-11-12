#pragma once
#include <memory>
#include "util.h"
namespace galois::gparallel {

class node;
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

private:
    friend class node;
    friend class node_container;

    bool _has_item_input;
    batch_function_type _batch_fn;
    query_function_type _query_fn;
    end_function_type _end_fn;
    int _node_id;
    int _node_user_id;
    int _item_deps_count;
    int _query_deps_count;
    std::string _name;
    data_link_vec _item_in;
    data_link_vec _item_out;
    std::vector<const node_info*> _item_node_in;
    std::vector<const node_info*> _item_node_out;
    data_link_vec _query_in;
    data_link_vec _query_out;
    std::vector<const node_info*> _query_node_in;
    std::vector<const node_info*> _query_node_out;
    data_link_vec _data_in;
    data_link_vec _data_out;
};
}