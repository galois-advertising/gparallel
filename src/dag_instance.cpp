// solopointer1202@gmail.com
#include <memory>
#include "dag_instance.h"
#include "node_instance.h"
#include "log.h"

namespace galois::gparallel {
dag_instance_ptr dag_instance::create(const dag_schema & schema) {
    auto _dag = new dag_instance(schema);
    return std::shared_ptr<dag_instance>(_dag);
}

dag_instance::dag_instance(const dag_schema & schema) {
    std::map<int, node_instance_ptr> schema_2_instance;
    // 1. create all node_instance
    nodes.reserve(schema.size());
    for (auto schema_node : schema) {
        auto node = node_instance::create(std::const_pointer_cast<const node_schema>(schema_node));
        schema_2_instance[schema_node->node_id()] = node;
        nodes.push_back(node);
    }
    // 2. connect them
    for (auto node : nodes) {
        for (auto output_node_schema : node->schema().output_nodes()) {
            auto itr = schema_2_instance.find(output_node_schema->node_id());
            if (itr == schema_2_instance.end()) {
                FATAL("there some error in connecting node_instances", "");
            }
            node->mutable_next_nodes().push_back(itr->second);
        }
    }
    

}

}