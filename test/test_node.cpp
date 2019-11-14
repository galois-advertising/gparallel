#include <iostream>
#include <gtest/gtest.h>
#include <stdio.h>
#include "node_deduce.h"
#include "util.h"
#include "meta.h"
using namespace galois::gparallel;

struct base {};

DECL_DATA_META(A, base) {
};

DECL_DATA_META(B, base, A) {
};

DECL_DATA_META(C, base, B) {
};

DECL_DATA_META(D, base, A, B, C) {
};

struct Process
{
    int process(input<A> a, input<B> b, output<C> c, output<D> d)
    {
        return 0;
    };
};

#define SHOW(_name) \
printf(#_name"\n");\
for (auto & a : _name) {\
    printf("\tkey:[%d] id:[%d] type:[%d] name:[%s]\n", \
        a.first, a.second.id, a.second.type, \
        typeid_manager<none_type>::instance().name(a.second.id).c_str());\
}\

TEST(Test, deduce_depends) {
    using namespace galois::gparallel;
    io_description vec;
    deduce_depends<auto_type, Process>::deduce(vec);
    SHOW(vec.item_input);
    SHOW(vec.item_output);
    SHOW(vec.query_output);
    SHOW(vec.query_output);
}