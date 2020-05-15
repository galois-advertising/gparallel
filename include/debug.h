// solopointer1202@gmail.com
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "dag_schema.h"


namespace galois::gparallel::debug
{
void show_meta_depends_graphviz(topology_t & meta_transitive_closure, std::string tag);

template<class meta_storage_t>
void show_node_depends_graphviz(const dag_schema<meta_storage_t>& _nodes, std::string tag) {
    std::stringstream node_depends_log;
    node_depends_log<<"\ndigraph "<<tag<<"{\n";
    node_depends_log<<"rankdir=BT;\n";
    node_depends_log<<"size=\"8,5\";\n";
    for (auto node : _nodes) {
        node->graphviz(node_depends_log);
    }
    node_depends_log<<"}";
    auto log_str = node_depends_log.str();
    std::regex re("galois::gparallel::none_type, |galois::gparallel::meta_info_t");
    INFO("%s\nhttp://graphviz.it/#", tag.c_str());
    INFO("%s", std::regex_replace(log_str, re, "").c_str());
}
bool graphviz(topology_t & target, std::stringstream & ss, std::string tag);
}