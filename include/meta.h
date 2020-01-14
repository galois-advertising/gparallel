#pragma once
#include "util.h"
#include "log.h"

namespace galois::gparallel {
// T: Data meta imp
// DMT: Data meta type
// M: Data meta template
// MS: Other M which M depends
template <class T, class MT, 
    template <class> class M, 
    template <class> class... MS_dep>
struct meta_traits : public T{
    typedef meta_info_list<MT, M, MS_dep...> meta_info;
    typedef MT meta_storage;
};
#define DECL_META(name, meta_storage, ms_dep...) template <class T> \
struct name : public meta_traits<T, meta_storage, name, ##ms_dep>
// DECL_MEAT(MetaName, MetaType, OtherMetaName...)
// MetaName<auto_type>[getters, setters] -> meta_traits[meta_info, meta_storage] -> auto_type    
// `data_meta_name` is essentially a T.
// Between `data_meta_name` and T, there a meta_traits 
// for describe: 
// 1. Other data_meta_names which it depends.
// 2. Type of this data_meta 


template <int I, class T>
struct meta_storage {
    meta_storage() = delete;
    template <class U>
    meta_storage(const U & u) : data(u) {
        INFO("[ENTRY] meta_storage<%d, %s>::meta_storage<%s>()",
            I, demangle(typeid(T).name()).c_str(), demangle(typeid(U).name()).c_str());
    }
    T data;
};

template <int I, class... TS>
struct storage_helper {};

template <int I, class T, class... TS>
struct storage_helper <I, T, TS...> : 
    public meta_storage <I, T>, 
    public storage_helper<I + 1, TS...> {
    template <class U>
    storage_helper(const U & u) : meta_storage<I, T>(u), storage_helper<I + 1, TS...>(u) {

    };
};

template <int I>
struct storage_helper<I> {
    template <class U> 
    storage_helper(const U &) {

    }
};

template <int I, class T>
T* get_state(meta_storage<I, T> & states) {
    return &states.data;
}

}