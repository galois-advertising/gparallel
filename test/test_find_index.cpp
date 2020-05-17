
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "gparallel.h"
#include "log.h"

struct AA {
    std::string* data;
    AA(std::string* a) : data(a) {}
};

TEST(Test, find_index) {
    using namespace galois::gparallel;
    ASSERT_EQ((find_index<int, 0, double, float, std::string, int>::index), 0);
    ASSERT_EQ((find_index<std::string, 0, double, float, std::string, int>::index), 2);
    ASSERT_EQ((find_index<AA, 0, double, float, std::string*, int>::index), 2);
}