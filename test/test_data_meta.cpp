#include <string>
#include <iostream>
#include <cxxabi.h>
#include <gtest/gtest.h>
#include "meta.h"
#include "meta_decorator.h"
#include "log.h"
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
    thread_data(int a) {}
};

DECL_META(m_aaa, thread_data){
    const int & AAA_storage() const { return this->data->AAA_storage;}
};

DECL_META(m_aab, thread_data){};
DECL_META(m_aac, thread_data){};
DECL_META(m_aa, thread_data, m_aaa, m_aab, m_aac){};

DECL_META(m_aba, thread_data){};
DECL_META(m_abb, thread_data){};
DECL_META(m_abc, thread_data){};
DECL_META(m_ab, thread_data, m_aba, m_abb, m_abc){};

DECL_META(m_aca, thread_data){};
DECL_META(m_acb, thread_data){};
DECL_META(m_acc, thread_data){};
DECL_META(m_ac, thread_data, m_aca, m_acb, m_acc){};

DECL_META(m_a, thread_data, m_aa, m_ab, m_ac){};

TEST(Test, meta_deduce) {
    using namespace galois::gparallel;
    io_description deps;
    input<m_a>::input_imp::deduce<none_type>(deps);
    thread_data td(1);
    input<m_a> input_a(&td);

    //storage_helper<0, input<m_aa>, input<m_ab>, output<m_ac>> storage(a);
}