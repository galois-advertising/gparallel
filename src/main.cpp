#include <iostream>
#include "util.h"

using namespace galois::gparallel;

struct data {
    int a;
    int b;
    int c;
    int d;
    data(int _a, int _b, int _c, int _d) : 
        a(_a), b(_b), c(_c), d(_d)
    {}
};

struct A {typedef data data_type; int a=1;A(const data * _a):a(_a->a){};};
struct B {int a=2;B(const data * _a):a(_a->b){};};
struct C {int a=3;C(const data * _a):a(_a->c){};};
struct D {int a=4;D(const data * _a):a(_a->d){};};

int main()
{
    data alldata[4] = {data(1,2,3,4), data(5,6,7,8), data(1,3,2,4), data(5,7,6,8)};
    glist<A,B,C,D> lst(alldata, 4);
    for (auto node : lst) {
        A a = node;
        B b = node;
        C c = node;
        D d = node;
        std::cout<<a.a<<"|"<<b.a<<"|"<<c.a<<"|"<<d.a<<std::endl;
    }
    return 0;
}