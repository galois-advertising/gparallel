#include <string>
#include <iostream>
#include <cxxabi.h>
#include <gtest/gtest.h>
#include "meta.h"
#include "meta_deduce.h"
using namespace galois::gparallel;
struct Adv {
    int advid = 1;
};

DECL_DATA_META(AAA, Adv){};
DECL_DATA_META(AAB, Adv){};
DECL_DATA_META(AAC, Adv){};
DECL_DATA_META(AA, Adv, AAA, AAB, AAC){};

DECL_DATA_META(ABA, Adv){};
DECL_DATA_META(ABB, Adv){};
DECL_DATA_META(ABC, Adv){};
DECL_DATA_META(AB, Adv, ABA, ABB, ABC){};

DECL_DATA_META(ACA, Adv){};
DECL_DATA_META(ACB, Adv){};
DECL_DATA_META(ACC, Adv){};
DECL_DATA_META(AC, Adv, ACA, ACB, ACC){};

DECL_DATA_META(A, Adv, AA, AB, AC){};

TEST(Test, data_meta) {
    using namespace galois::gparallel;
    using namespace galois::gparallel::meta;
    io_description deps;
    deduce_depends<galois::gparallel::output<A>::meta_info>::deduce<none_type>(deps);
}