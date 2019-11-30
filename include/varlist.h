#pragma once
#include <memory>


namespace galois::gparallel
{
// I: index
// T: single type
// TS: type list
template <int I, class T>
struct var_storage {
    template <class U>
    var_storage(U u) : data(u) {}
    std::shared_ptr<T> data;
};
template <int I, class... TS>
struct varlist{};

template <int I, class T, class... TS>
struct varlist<I, T, TS...> : 
    public varlist<I + 1, TS...>, 
    public var_storage<I, T> {
    template <class U>
    varlist(U* u) : varlist<I + 1, TS...>(u), var_storage<I, T>(u) {};
};

template <int I>
struct varlist<I> {
    template <class U> 
    varlist(U*) {}
};

template <int I, class T>
std::shared_ptr<T> get_var(var_storage<I, T> & st) {
    return st.data;
};

}