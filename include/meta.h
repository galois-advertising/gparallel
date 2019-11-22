#pragma once
#include "util.h"

namespace galois::gparallel {
// IMP: Data meta imp
// DMT: Data meta type
// M: Data meta template
// MS: Other M which M depends
template <class IMP, class MDT, 
    template <class> class M, 
    template <class> class... MS_dep>
struct meta_traits : public IMP {
    typedef meta_info_list<IMP, M, MS_dep...> meta_info;
    typedef MDT data_meta_type;
};
#define DECL_META(name, data_meta_type, ms_dep...) template <class IMP> struct name \
: public meta_traits<IMP, data_meta_type, name, ##ms_dep>

// T->meta_traits->data_meta_name
// `data_meta_name` is essentially a T.
// Between `data_meta_name` and T, there a meta_traits 
// for describe: 
// 1. Other data_meta_names which it depends.
// 2. Type of this data_meta 

}