#pragma once
#include "meta_decorator.h"
namespace galois::gparallel {

template <class M>
struct parameter_traits {
};

template < template <class> class M>
struct parameter_traits< input<M> > {
    typedef type_id<typename M<none_type>::meta_info, none_type> node_meta_id;
    typedef typename M<none_type>::meta_info meta_info;
    static const parameter_type ptype = parameter_type::INPUT;
    static int id() {
        return node_meta_id::instance().id();
    }
    static const char * name() {
        return node_meta_id::instance().name();
    }
};

template <template <class> class M>
struct parameter_traits< output<M> > {
    typedef type_id<typename M<none_type>::meta_info, none_type> node_meta_id;
    typedef typename M<none_type>::meta_info meta_info;
    static const parameter_type ptype = parameter_type::OUTPUT;
    static int id() {
        return node_meta_id::instance().id();
    }
    static const char* name() {
        return node_meta_id::instance().name();
    }
};

template <template <class> class M>
struct parameter_traits< produce<M> > {
    typedef type_id<typename M<none_type>::meta_info, none_type> node_meta_id;
    typedef typename M<none_type>::meta_info meta_info;
    static const parameter_type ptype = parameter_type::PRODUCE;
    static int id() {
        return node_meta_id::instance().id();
    }
    static const char* name() {
        return node_meta_id::instance().name();
    }
};
}