#include <iostream>
#include <gtest/gtest.h>
#include "type_id.h"
struct test_tag_1 {};
struct test_tag_2 {};
TEST(Test, typeid) {
    using namespace galois::gparallel;
    std::cout<<type_id<int, test_tag_1>::instance().id()<<std::endl;
    std::cout<<type_id<int, test_tag_1>::instance().name()<<std::endl;
    std::cout<<type_id<double, test_tag_1>::instance().id()<<std::endl;
    std::cout<<type_id<double, test_tag_1>::instance().name()<<std::endl;
    std::cout<<type_id<double, test_tag_2>::instance().id()<<std::endl;
    std::cout<<type_id<double, test_tag_2>::instance().name()<<std::endl;
}