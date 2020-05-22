#include <iostream>
#include "invoke_ex.h"
#include <gtest/gtest.h>

struct A{int value=1;};
struct B{int value=2;};
struct C{int value=3;};
struct D{int value=4;};
struct E{int value=5;};

int fn(A a, B b, C c, D d)
{
    std::cout<<a.value<<b.value<<c.value<<d.value<<std::endl;
    return 1;
}

TEST(Test, invoke_ex) {
    //using namespace galois::gparallel;
    //A a;
    //B b;
    //C c;
    //D d;
    //// any order of a,b,c,d is ok
    //ASSERT_EQ(invoke_ex(fn, a, b, c, d), 1);
    //ASSERT_EQ(invoke_ex(fn, d, b, c, a), 1);
    //ASSERT_EQ(invoke_ex(fn, d, c, b, a), 1);
    //ASSERT_EQ(invoke_ex(fn, d, c, b, a), 1);
}

struct AA {
    float data;
    AA(float a) : data(a) {}
};

struct BB {
    int data;
    BB(int a) : data(a) {}
};

struct CC {
    float data;
    CC(float a) : data(a) {}
};

struct DD {
    int data;
    DD(int a) : data(a) {}
};

int fn2(AA a, BB b, CC c, DD d) {
    std::cout<<a.data<<b.data<<c.data<<d.data<<std::endl;
    return 1;
}

TEST(Test, invoke_ex2) {
    using namespace galois::gparallel;
    // any order of a,b,c,d is ok
    //ASSERT_EQ(invoke_ex(fn2, 1, 1.1), 1);
}