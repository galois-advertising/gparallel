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

DECL_META(AAA, Adv){};
DECL_META(AAB, Adv){};
DECL_META(AAC, Adv){};
DECL_META(AA, Adv, AAA, AAB, AAC){};

DECL_META(ABA, Adv){};
DECL_META(ABB, Adv){};
DECL_META(ABC, Adv){};
DECL_META(AB, Adv, ABA, ABB, ABC){};

DECL_META(ACA, Adv){};
DECL_META(ACB, Adv){};
DECL_META(ACC, Adv){};
DECL_META(AC, Adv, ACA, ACB, ACC){};

DECL_META(A, Adv, AA, AB, AC){};

TEST(Test, data_meta) {
    using namespace galois::gparallel;
    using namespace galois::gparallel::meta;
    io_description deps;
    deduce_depends<output<A>::meta_info>::deduce<none_type>(deps);
}