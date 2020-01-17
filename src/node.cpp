#include "node.h"
namespace galois::gparallel {
node_ptr node::create(const node_schema_cptr) {
    return std::make_shared<node>();
}
}