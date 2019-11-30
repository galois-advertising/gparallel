#pragma once
#include "util.h"
#include "push_io.h"
#include "meta_decorator.h"

namespace galois::gparallel {

template <class C, class... MS>
struct input_meta_imp_op {};
template <class C, class... MS>
struct input_meta_imp_dd {};


// C: An abstract type, which story some info
// D: Data type
// M: Data meta type
// MS_dep: The data meta types which depends it
// template_list: a list of any template
// meta_info_list: a special list for store <D, M, MS_dep ...>
//
// deducer<C, meta_info_list>
template <class C, class D, template <class> class M, template <class> class... MS_dep>
struct input_meta_imp_dd<C, meta_info_list<D, M, MS_dep ...>> 
    : public input_meta_imp_op<C, template_list<MS_dep...>> 
{
    template <class P>
    static void deduce(io_description & deps) {
        input_meta_imp_op<C, template_list<MS_dep...>>::template deduce<P>(deps);
    }
};

// end case
template <class C>
struct input_meta_imp_op<C, template_list<>> : public C {
    template <class P>
    static void deduce(io_description & deps) {
        C::template deduce<P>(deps);
    }
};

// deducer<C, template_list>
template <class C, template <class> class M, template <class> class... MS_dep>
struct input_meta_imp_op<C, template_list<M, MS_dep ...>> 
    : public M<input_meta_imp_dd<input_meta_imp_op<C, template_list<MS_dep ...>>, typename M<none_type>::meta_info>> 
{
    template <class P>
    static void deduce(io_description & deps) {
        push_io<input<M>, P>(deps);
        input_meta_imp_dd<input_meta_imp_op<C, template_list<MS_dep ...>>, typename M<none_type>::meta_info>::template deduce<P>(deps);
    }
};

// start
template <class T> struct input_meta_imp {};
template <class D, template <class> class M, template <class> class... MS_dep>
struct input_meta_imp< meta_info_list<D, M, MS_dep ...> > 
    : public M<input_meta_imp_op<data_wrapper<D>, template_list<MS_dep ...>>> 
{
    input_meta_imp(const D * data) {
        this->reset(const_cast<D*>(data)); 
    }
    template <class P>
    static void deduce(io_description & deps) {
        input_meta_imp_op<data_wrapper<D>, template_list<MS_dep ...>>::template deduce<P>(deps);
    };
};


}