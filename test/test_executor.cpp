#include <iostream>
#include <gtest/gtest.h>
#include <stdio.h>
#include "gparallel.h"
using namespace galois::gparallel;

struct base {};

DECL_META(A, base) {
};

DECL_META(B, base, A) {
};

DECL_META(C, base) {
};

struct Process
{
    static void process(input<A> a, input<B> b, output<C> c) {
        TRACE("[gparallel] process succeed", "");
    };
};

TEST(Test, executor) {
    using namespace galois::gparallel;
    base b;
    auto exe = executor<base, decltype(Process::process)>::create(Process::process);
    exe(&b);

}