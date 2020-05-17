#include <iostream>
#include <gtest/gtest.h>
#include <stdio.h>
#include <algorithm>
#include "gparallel.h"
using namespace galois::gparallel;

struct base {};
DECL_META(MetaB, base) {};

DECL_META(MetaC, base) {};

DECL_META(MetaA, base, MetaC) {};

DECL_META(MetaD, base) {};

DECL_META(MetaE, base, MetaD) {};

struct NodeA
{
    static int process(input<MetaA> b) { return 0; }
};

struct NodeB
{
    static int process(input<MetaA> a, output<MetaB> b) { return 0; }
};

struct NodeC
{
    static int process(output<MetaC> c) { return 0; }
};

struct NodeD
{
    static int process(input<MetaB> b, input<MetaC> c, output<MetaD> d) { return 0; }
};

struct NodeE
{
    static int process(output<MetaE> e) { return 0; }
};

TEST(Test, register_node) {
    using namespace galois::gparallel;
    dag_schema<base> nodes;
    register_node<base, NodeA, NodeB, NodeC, NodeD, NodeE>::reg(nodes);
    ASSERT_EQ(setup_dag_schema<base>(nodes), true);
}
