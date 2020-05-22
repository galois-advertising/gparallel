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
}