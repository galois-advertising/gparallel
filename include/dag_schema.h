#pragma once
#include <string>
#include <vector>
#include <memory>
#include "node_deduce.h"
#include "node_info.h"
#include "util.h"


namespace galois::gparallel
{

typedef std::map< id_t, std::vector<node_ptr> > meta_to_nodevec_t;
typedef std::map< id_t, node_ptr > meta_to_node_t;
typedef std::vector<node_ptr> dag_schema;

bool setup_dag_schema(dag_schema & _nodes);
bool build_meta_topology(const dag_schema & _nodes, topology_t & me);
bool build_node_topology(const dag_schema & _nodes, topology_t & me);
bool transitive_closure(topology_t & implies);

template <class NT>
void register_node_operator(dag_schema & c)
{
    std::string name = demangle(typeid(NT).name());
    auto new_node = std::make_shared<node_info>();
    c.push_back(new_node);
    io_description vec;
    deduce_depends<auto_type, NT>::deduce(vec);
    //_nodes.back()->initialize(strdup(name.c_str()), batch_fn, query_fn, end_fn, vec);
    c.back()->initialize(name, nullptr, nullptr, nullptr, vec);
}

template <class ...NTS>
struct register_node {
    static void reg(dag_schema & c)
    {
        register_node<type_list<NTS ...>>::reg(c);
    };
};

template <class NT, class ...NTS>
struct register_node<type_list<NT, NTS ...>> {
    static void reg(dag_schema & c)
    {
        register_node_operator<NT>(c);
        register_node<NTS ...>::reg(c);
    };
};

template <>
struct register_node<type_list<>> {
    static void reg(dag_schema & ) { /* stop recursion */}
};

}
