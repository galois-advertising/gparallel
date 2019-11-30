#include <string>
#include <iostream>
#include <cxxabi.h>
#include <gtest/gtest.h>
#include "meta.h"
#include "meta_decorator.h"
using namespace galois::gparallel;
struct thread_data {
    int advid;
    thread_data(const int & a) : advid(a) {}
};

DECL_META(AAA, thread_data){
    int value;
    AAA(const int & a) : value(a) {}
};
DECL_META(AAB, thread_data){
    int value;
    AAB(const int & a) : value(a) {}
};
DECL_META(AAC, thread_data){
    int value;
    AAC(const int & a) : value(a) {}
};
DECL_META(AA, thread_data, AAA, AAB, AAC){};

DECL_META(ABA, thread_data){};
DECL_META(ABB, thread_data){};
DECL_META(ABC, thread_data){};
DECL_META(AB, thread_data, ABA, ABB, ABC){};

DECL_META(ACA, thread_data){};
DECL_META(ACB, thread_data){};
DECL_META(ACC, thread_data){};
DECL_META(AC, thread_data, ACA, ACB, ACC){};

DECL_META(A, thread_data, AA, AB, AC){};

TEST(Test, data_meta) {
    using namespace galois::gparallel;
    io_description deps;
    input_meta_imp<input<A>::meta_info>::deduce<none_type>(deps);
    thread_data td(1);
    input<A> input_a(&td);

    //storage_helper<0, input<AA>, input<AB>, output<AC>> storage(a);
}