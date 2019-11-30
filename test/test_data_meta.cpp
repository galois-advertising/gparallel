#include <string>
#include <iostream>
#include <cxxabi.h>
#include <gtest/gtest.h>
#include "meta.h"
#include "meta_decorator.h"
using namespace galois::gparallel;
struct thread_data {
    int advid;
    thread_data(int a) : advid(a) {}
};
typedef const thread_data * cp_thread_data;

DECL_META(AAA, cp_thread_data){
    int value;
    AAA(const int & a) : value(a) {}
};
DECL_META(AAB, cp_thread_data){
    int value;
    AAB(const int & a) : value(a) {}
};
DECL_META(AAC, cp_thread_data){
    int value;
    AAC(const int & a) : value(a) {}
};
DECL_META(AA, cp_thread_data, AAA, AAB, AAC){};

DECL_META(ABA, cp_thread_data){};
DECL_META(ABB, cp_thread_data){};
DECL_META(ABC, cp_thread_data){};
DECL_META(AB, cp_thread_data, ABA, ABB, ABC){};

DECL_META(ACA, cp_thread_data){};
DECL_META(ACB, cp_thread_data){};
DECL_META(ACC, cp_thread_data){};
DECL_META(AC, cp_thread_data, ACA, ACB, ACC){};

DECL_META(A, cp_thread_data, AA, AB, AC){};

TEST(Test, data_meta) {
    using namespace galois::gparallel;
    io_description deps;
    input_meta_imp<input<A>::meta_info>::deduce<none_type>(deps);
    thread_data td(1);
    input<A> input_a(&td);

    //storage_helper<0, input<AA>, input<AB>, output<AC>> storage(a);
}