#include <iostream>
#include <gtest/gtest.h>
#include <stdio.h>
#include <algorithm>
#include "gparallel.h"
using namespace galois::gparallel;

struct base {};

DECL_META(MetaB, base) {};

DECL_META(MetaC, base) {};

DECL_META(MetaA, base, MetaC) {};

DECL_META(MetaD, base) {};

DECL_META(MetaE, base, MetaD) {};

TEST(Test, varlist) {
    using namespace galois::gparallel;
    //varlist<0, MetaA<none_type>, MetaB<none_type>, MetaC<none_type>, MetaD<none_type>> list;

}
