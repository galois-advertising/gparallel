
#pragma once

namespace galois::gparallel {

template <template<class> class M> class input; 
template <template<class> class M> class output;
template <template<class> class M> class produce;
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

template <class A, class NT>
void push_io(io_description & iodes) {
    if (parameter_traits<A>::ptype == parameter_type::NONE) {
        return;
    }
    bool is_item = true;
    io_item link;
    link.id = parameter_traits<A>::id();
    link.type = parameter_traits<A>::ptype;
    link.meta_level = is_item ? meta_level_t::ITEM : meta_level_t::QUERY;

    if (link.type == parameter_type::INPUT) {
        iodes.input[link.id] = link;
    } else {
        iodes.output[link.id] = link;
    }
};
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
    : public M<input_meta_imp_op<storage_reference<D>, template_list<MS_dep ...>>> 
{
    input_meta_imp(const D * data) {
        this->reset(const_cast<D*>(data)); 
    }
    template <class P>
    static void deduce(io_description & deps) {
        input_meta_imp_op<storage_reference<D>, template_list<MS_dep ...>>::template deduce<P>(deps);
    };
};



template <template<class> class M>
class input {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::meta_storage data_meta_storage;
    typedef input_meta_imp<meta_info> input_imp;
    input(const data_meta_storage * data) : _m(const_cast<data_meta_storage*>(data)) {}
    template <template <class> class AnyM>
    operator input<AnyM>() {
        return input<AnyM>(_m.data());
    }
private:
    input_imp _m;
};

template <template<class> class M>
class output {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::meta_storage data_meta_storage;
    typedef M<storage_reference<data_meta_storage>> * meta_imp_type;

    struct output_storage_t : public M<storage_reference<data_meta_storage>> {
        output_storage_t (data_meta_storage * data) { this->reset(data); }
    };

    output(meta_imp_type v) : _v(v) {}
    meta_imp_type _v;
};

template <template<class> class M>
class produce {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::meta_storage data_meta_storage;
};

}