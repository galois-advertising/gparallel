// solopointer1202@gmail.com
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "dag_schema.h"


namespace galois::gparallel::debug
{
void show_meta_depends_graphviz(topology_t & meta_transitive_closure, std::string tag);
void show_node_depends_graphviz(const dag_schema & _nodes, std::string tag);
bool graphviz(topology_t & target, std::stringstream & ss, std::string tag);
}