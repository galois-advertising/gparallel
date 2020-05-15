// solopointer1202@gmail.com
#pragma once
#include "util.h"
#include "log.h"

namespace galois::gparallel {

template <class T, class _meta_storage_t, 
    template <class> class _meta_name, 
    template <class> class... _super_meta_names>
struct meta_traits : public T {
    typedef meta_info_t<_meta_storage_t, _meta_name, _super_meta_names...> meta_info;
    typedef _meta_storage_t meta_storage_t;
};
#define DECL_META(meta_name, meta_storage_t, super_meta_names...) template <class T> \
struct meta_name : public meta_traits<T, meta_storage_t, meta_name, ##super_meta_names>
// DECL_MEAT(MetaName, MetaType, OtherMetaName...)
// MetaName<auto_type>[getters, setters] -> meta_traits[meta_info, meta_storage_t] -> auto_type    
// `data_meta_name` is essentially a T.
// Between `data_meta_name` and T, there a meta_traits 
// for describe: 
// 1. Other data_meta_names which it depends.
// 2. Type of this data_meta 


//template <int I, class T>
//struct meta_storage_t {
//    meta_storage_t() = delete;
//    template <class U>
//    meta_storage_t(const U & u) : data(u) {
//        INFO("[ENTRY] meta_storage_t<%d, %s>::meta_storage_t<%s>()",
//            I, demangle(typeid(T).name()).c_str(), demangle(typeid(U).name()).c_str());
//    }
//    T data;
//};

//template <int I, class... TS>
//struct storage_helper {};
//
//template <int I, class T, class... TS>
//struct storage_helper <I, T, TS...> : 
//    public meta_storage_t <I, T>, 
//    public storage_helper<I + 1, TS...> {
//    template <class U>
//    storage_helper(const U & u) : 
//        meta_storage_t<I, T>(u), storage_helper<I + 1, TS...>(u) {};
//};
//
//template <int I>
//struct storage_helper<I> {
//    template <class U> 
//    storage_helper(const U &) {}
//};

//template <int I, class T>
//T* get_state(meta_storage_t<I, T> & states) {
//    return &states.data;
//}

}