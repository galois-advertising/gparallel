#include "dag.h"
#include "node.h"
#include <memory>

namespace galois::gparallel {
dag_ptr dag::create(const dag_schema & schema) {
    auto _dag = new dag(schema);
    return std::shared_ptr<dag>(_dag);
}

dag::dag(const dag_schema & schema) {
    std::map<int, node_ptr> node_id_map;
    nodes.reserve(schema.size());
    for (auto schema_node : schema) {
        auto node = node::create(std::const_pointer_cast<const node_schema>(schema_node));
        //node_id_map[node->node_id()] = node;
        nodes.push_back(node);
    }
    

}

}