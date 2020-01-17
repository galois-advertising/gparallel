#pragma once
#include <string>
#include <vector>
#include <memory>
#include "util.h"
#include "dag_schema.h"
#include "node.h"


namespace galois::gparallel
{
class dag;
typedef std::shared_ptr<dag> dag_ptr;
typedef std::shared_ptr<const dag> dag_cptr;
class dag {
public:
    static dag_ptr create(const dag_schema &);
private:
    dag(const dag_schema &);
    std::vector<node_ptr> nodes;
};
}