#pragma once
#include "util.h"

namespace galois::gparallel::meta {
using namespace galois::gparallel;

template <class C, class... MS>
struct deducer_op {};
template <class C, class... MS>
struct deducer_dd {};


// C: An abstract type, which story some info
// D: Data yype
// M: Data meta type
// MS_dep: The data meta types which depends it
// template_list: a list of any template
// meta_info_list: a special list for store <D, M, MS_dep ...>
//
// deducer<C, meta_info_list>
template <class C, class D, template <class> class M, template <class> class... MS_dep>
struct deducer_dd<C, meta_info_list<D, M, MS_dep ...>> 
    : public deducer_op<C, template_list<MS_dep...>> 
{
    template <class P>
    static void deduce(io_description & deps) {
        deducer_op<C, template_list<MS_dep...>>::template deduce<P>(deps);
    }
};

// end case
template <class C>
struct deducer_op<C, template_list<>> : public C {
    template <class P>
    static void deduce(io_description & deps) {
        C::template deduce<P>(deps);
    }
};

// deducer<C, template_list>
template <class C, template <class> class M, template <class> class... MS_dep>
struct deducer_op<C, template_list<M, MS_dep ...>> 
    : public M<deducer_dd<deducer_op<C, template_list<MS_dep ...>>, typename M<none_type>::meta_info>> 
{
    template <class P>
    static void deduce(io_description & deps) {
        push_io<input<M>, P>(deps);
        deducer_dd<deducer_op<C, template_list<MS_dep ...>>, typename M<none_type>::meta_info>::template deduce<P>(deps);
    }
};

// start
template <class T> struct deduce_depends {};
template <class D, template <class> class M, template <class> class... MS_dep>
struct deduce_depends< meta_info_list<D, M, MS_dep ...> > 
    : public M<deducer_op<data_wrapper<D>, template_list<MS_dep ...>>> 
{
    template <class P>
    static void deduce(io_description & deps) {
        deducer_op<data_wrapper<D>, template_list<MS_dep ...>>::template deduce<P>(deps);
    };
};


}