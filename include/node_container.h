#pragma once
#include <string>
#include <vector>
#include <memory>
#include "node_deduce.h"
#include "node_info.h"
#include "util.h"


namespace galois::gparallel
{

typedef std::set<meta_id_t> meta_id_set_t;
typedef std::set<node_id_t> node_id_set_t;
typedef std::map< meta_id_t, std::vector<node_ptr> > meta_to_nodevec_t;
typedef std::map< meta_id_t, node_ptr > meta_to_node_t;
class node_container
{
public: 
    bool init();
    template <class NT>
    void register_node_operator();
    auto begin(){return _nodes.begin();}
    auto end(){return _nodes.end();}
private:
    void show_meta_depends_graphviz(
    std::map<meta_id_t, meta_id_set_t> & meta_implies, std::string tag);
    bool build_meta_depends(std::map<meta_id_t, meta_id_set_t> & meta_implies);
    std::vector<node_ptr> _nodes;
    //std::vector<node_ptr> _end_nodes;
    //std::map<std::string, node_ptr> _name_node_map;
};

template <class ...NTS>
struct register_node {
    static void reg(node_container & c)
    {
        register_node<type_list<NTS ...>>::reg(c);
    };
};

template <class NT, class ...NTS>
struct register_node<type_list<NT, NTS ...>> {
    static void reg(node_container & c)
    {
        c.register_node_operator<NT>();
        register_node<NTS ...>::reg(c);
    };
};

template <>
struct register_node<type_list<>> {
    static void reg(node_container & ) { /* stop recursion */}
};

template <class NT>
void node_container::register_node_operator()
{
    std::string name = demangle(typeid(NT).name());
    auto new_node = std::make_shared<node_info>();
    _nodes.push_back(new_node);
    io_description vec;
    deduce_depends<auto_type, NT>::deduce(vec);
    //_nodes.back()->initialize(strdup(name.c_str()), batch_fn, query_fn, end_fn, vec);
    _nodes.back()->initialize(name, nullptr, nullptr, nullptr, vec);
}
}
