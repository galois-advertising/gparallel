#include <iostream>
#include "invoke_ex.h"
#include <gtest/gtest.h>

struct A{int value=1;};
struct B{int value=2;};
struct C{int value=3;};
struct D{int value=4;};

int fn(A a, B b, C c, D d)
{
    std::cout<<a.value<<b.value<<c.value<<d.value<<std::endl;
    return 1;
}

TEST(Test, invoke_ex) {
    using namespace galois::gparallel;
    A a;
    B b;
    C c;
    D d;
    // any order of a,b,c,d is ok
    std::cout<<invoke_ex(fn, a, b, c, d)<<std::endl;
    std::cout<<invoke_ex(fn, d, b, c, a)<<std::endl;
    std::cout<<invoke_ex(fn, d, c, b, a)<<std::endl;
}