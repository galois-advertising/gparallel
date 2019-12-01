#include <string>
#include <iostream>
#include <cxxabi.h>
#include <gtest/gtest.h>
#include "meta.h"
#include "meta_decorator.h"
using namespace galois::gparallel;
struct thread_data {
    int AAA_storage = 0;
    int AAB_storage = 0;
    int AAC_storage = 0;

    int ABA_storage = 0;
    int ABB_storage = 0;
    int ABC_storage = 0;

    int ACA_storage = 0;
    int ACB_storage = 0;
    int ACC_storage = 0;

    int AA_storage = 0;
    int AB_storage = 0;
    int AC_storage = 0;

    int A_storage = 0;
    thread_data(int a) : advid(a) {}
};

DECL_META(AAA, thread_data){
    const int & AAA() const { return this->data->AAA_storage;}
    const int & AAA() const { return this->data->AAA_storage;}
};

DECL_META(AAB, thread_data){};
DECL_META(AAC, thread_data){};
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