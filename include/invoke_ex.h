// solopointer1202@gmail.com
#pragma once
#include <type_traits>
#include "util.h"
/***************************************************************************
*  An extended version of std::invoke, support pass parameters in any order.
*  solopointer1202@gmail.com
*  2019.11.09 14:55:41
****************************************************************************/
// R: Type of return value
// F: Type of function
// T: Type
// I: One int 
// IS: Multiple int
// A: One argument's type
// AS: Type of multiple arguments
// CS: Type of multiple value candidates
// MS: Type of matched types

namespace galois::gparallel {

//template <class... type> struct type_list {};
template <class T, int I, class... AS>
struct find_index {
    typedef char type[I];
};
template <class T, int I, class A, class... AS>
struct find_index<T, I, A, AS...> {
    typedef char VT[I];
    static VT& test(T);
    static typename find_index<T, I + 1, AS...>::type& test(...);
    typedef decltype(test(A())) type;
    static const int index = sizeof(type);
};

template <int I, class T, class... TS>
struct n_th {
    typedef typename n_th<I - 1, TS...>::type type;

    static type value(T, TS ... ts) {
        return n_th<I - 1, TS...>::value(ts...);
    }
};

template <class T, class... TS>
struct n_th<0, T, TS...> {
    typedef T type;
    static type value(T t, ...) {
        return t;
    }
};


// followings handle parameter

template <class R, class F, class CS, class MS, int... IS>
struct parameter_match_invoke {};

template <class R, class F, class... CS, class... MS, int I, int... IS>
struct parameter_match_invoke<R, F, type_list<CS...>, type_list<MS...>, I, IS...> {
    typedef typename n_th<I, CS...>::type Match;
    static R process(F fn, CS... cs, MS ... ms) {
        return parameter_match_invoke<R, F, type_list<CS...>, type_list<MS..., Match>, IS...>::process(fn, 
            cs..., ms..., n_th<I, CS...>::value(cs...));
    }
};

template <class R, class... CS, class... MS, class... AS>
struct parameter_match_invoke<R, R(*)(AS...), type_list<CS...>, type_list<MS...>> {
    static R process(R(*fn)(AS...), CS ... , MS ... ms) {
        return fn(ms...);
    }
};

// parameter_match
template <class R, class F, class AS, class CS, int... IS>
struct parameter_match {};

template <class R, class F, class A, class... AS, class... CS, int... IS>
struct parameter_match<R, F, type_list<A, AS...>, type_list<CS...>, IS...> {
    static const int index = find_index<A, 0, CS...>::index;
    static_assert(index != sizeof...(CS), "not match!");
    static R process(F fn, CS... cs) {
        return parameter_match<R, F, type_list<AS...>, type_list<CS...>, IS..., index>::process(fn, cs...);
    }
};

template <class R, class F, class... CS, int... IS>
struct parameter_match<R, F, type_list<>, type_list<CS...>, IS...> {
    static R process(F fn, CS... cs) {
        return parameter_match_invoke<R, F, type_list<CS...>, type_list<>, IS...>::process(fn, cs...);
    }
};

// invoke_ex
template <class R, class... AS, class... CS>
R invoke_ex(R(*fn)(AS...), CS... cs) {
    return parameter_match<R, R(*)(AS...), type_list<AS...>, type_list<CS...>>::process(fn, cs...);
};

}
