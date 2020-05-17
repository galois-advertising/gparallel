// solopointer1202@gmail.com
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <regex>
#include "dag_schema.h"
#include "type_id.h"
#include "util.h"
#include "log.h"


namespace galois::gparallel {
void show_meta_depends_graphviz(topology_t & meta_transitive_closure, std::string tag) {
    std::stringstream meta_depends_log;
    meta_depends_log<<"\ndigraph "<<tag<<"{\n";
    meta_depends_log<<"rankdir=BT;\n";
    meta_depends_log<<"size=\"8,5\";\n";
    for (auto & p : meta_transitive_closure) {
        std::cout<<typeid_manager<none_type>::instance().name(p.first)<<std::endl;;
        for (auto id : p.second) {
                meta_depends_log<<"\""<<typeid_manager<none_type>::instance().name(p.first)
                <<"\" -> \""<<typeid_manager<none_type>::instance().name(id)<<"\""<<";"<<std::endl;
        }
    }
    meta_depends_log<<"}";
    auto log_str = meta_depends_log.str();
    std::regex re("galois::gparallel::none_type, |galois::gparallel::meta_info_t");
    INFO("%s\nhttp://graphviz.it/#", tag.c_str());
    INFO("%s", std::regex_replace(log_str, re, "").c_str());

}

bool graphviz(topology_t& target, std::stringstream & ss, std::string tag) {
    ss<<"digraph "<<tag<<"{"<<std::endl;
    ss<<"rankdir=BT;"<<std::endl;
    ss<<"size=\"8,5\";"<<std::endl;
    for (auto& [output, input_set] : target) {
        for (auto input : input_set) {
                ss<<"\""<<output<<"\" -> \""<<input<<"\""<<";"<<std::endl;
        }
    }
    ss<<"}";
    return true;
}
}