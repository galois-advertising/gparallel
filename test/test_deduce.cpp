#include <iostream>
#include <gtest/gtest.h>
#include <stdio.h>
#include "node_deduce.h"
#include "util.h"
#include "meta.h"
using namespace galois::gparallel;

struct base {};
DECL_DATA_META(MetaB, base) {};

DECL_DATA_META(MetaC, base) {};

DECL_DATA_META(MetaA, base, MetaC) {};

DECL_DATA_META(MetaD, base) {};

DECL_DATA_META(MetaE, base, MetaD) {};

struct NodeA
{
    int process(produce<MetaA> b) { return 0; }
};

struct NodeB
{
    int process(input<MetaA> a, output<MetaB> b) { return 0; }
};

struct NodeC
{
    int process(output<MetaC> c) { return 0; }
};

struct NodeD
{
    int process(input<MetaB> b, input<MetaC> c, output<MetaD> d) { return 0; }
};

struct NodeE
{
    int process(output<MetaE> e) { return 0; }
};

#define SHOW(_name) \
printf(#_name"\n");\
for (auto & a : _name) {\
    printf("\tkey:[%d] id:[%d] type:[%d] name:[%s]\n", \
        a.first, a.second.id, a.second.type, \
        typeid_manager<none_type>::instance().name(a.second.id).c_str());\
}\

void show(io_description & vec)
{
    SHOW(vec.item_input);
    SHOW(vec.item_output);
    SHOW(vec.query_output);
    SHOW(vec.query_output);
}

TEST(Test, deduce) {
    using namespace galois::gparallel;
    {
        std::cout<<"-----NodeA-----"<<std::endl;
        io_description vec;
        deduce_depends<auto_type, NodeA>::deduce(vec);
        show(vec);
    }
    {
        std::cout<<"-----NodeB-----"<<std::endl;
        io_description vec;
        deduce_depends<auto_type, NodeB>::deduce(vec);
        show(vec);
    }
    {
        std::cout<<"-----NodeC-----"<<std::endl;
        io_description vec;
        deduce_depends<auto_type, NodeC>::deduce(vec);
        show(vec);
    }
    {
        std::cout<<"-----NodeD-----"<<std::endl;
        io_description vec;
        deduce_depends<auto_type, NodeD>::deduce(vec);
        show(vec);
    }
    {
        std::cout<<"-----NodeE-----"<<std::endl;
        io_description vec;
        deduce_depends<auto_type, NodeE>::deduce(vec);
        show(vec);
    }
}