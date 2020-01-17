#pragma once
#include <memory>
#include "node_schema.h"
namespace galois::gparallel {
typedef std::shared_ptr<node> node_ptr;
typedef std::shared_ptr<const node> node_cptr;
class node {
public:
    static node_ptr create(node_schema_cptr);
    std::vector<node_ptr> next_nodes;
};
}