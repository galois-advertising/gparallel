#include "dag_schema.h"

namespace galois::gparallel {
std::optional<dag_schema> topological_sort(const dag_schema& const_dag) {
    auto dag = const_dag;
    dag_schema result;
    while(!dag.empty()) {
        auto res = std::find_if(dag.begin(), dag.end(), [](auto node)->bool{
            return node->mutable_input_nodes().size() == 0;
        });
        if (res != dag.end()) {
            TRACE("Found 0-input node[%s]", (*res)->name().c_str());
            result.push_back(*res);
            auto del = *res;
            dag.erase(res);
            std::for_each(dag.begin(), dag.end(), [del](auto node){
                auto res = node->delete_input_nodes(del);
                TRACE("Delete input [%s] for [%s] : [%s]", 
                    del->name().c_str(), node->name().c_str(),
                    res ? "true" : "false");
            });
        } else {
            return std::nullopt;
        }
    }
    return result;
}
}