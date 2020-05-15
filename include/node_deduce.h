// solopointer1202@gmail.com
#pragma once
#include <type_traits>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "util.h"
#include "meta_decorator.h"

namespace galois::gparallel
{
// NT: node type
// R: return type
// A: arg type
// AS: args type
// 1. simple deduce_arg detail
template <class A, class NT>
struct deduce_op {
    static void deduce(io_description & iodes) {
        record_input_output<A, NT>(iodes);
        depth_first_search_of_meta<typename parameter_traits<A>::meta_info>::template deduce<NT>(iodes);
    }
};

// 2.last deduce_arg in the recursion 
template <class NT, class...AS>
struct deduce_arg {
    static void deduce(io_description &) {
        // do nothing
        // the stop template in the recursion
    }
};

template <class NT, class A, class...AS>
struct deduce_arg<NT, A, AS...> {
    static void deduce(io_description & iodes) {
        deduce_op<A, NT>::deduce(iodes);
        deduce_arg<NT, AS...>::deduce(iodes);
    }
};

// depends deduce
// default template
template <class...AS>
struct deduce_fn {};
// for normal function
template <class NT, class R, class...AS>
struct deduce_fn<NT, R(*)(AS...)> : 
    public deduce_arg<NT, AS...> {};
// for nember function
template <class NT, class R, class...AS>
struct deduce_fn<NT, R(NT::*)(AS...)> : 
    public deduce_arg<NT, AS...> {};

// start from here
template <class NT>
struct deduce_depends {
    static void deduce(io_description & vec) {
        deduce_fn<NT, decltype(&NT::process)>::deduce(vec);
    }
};

};