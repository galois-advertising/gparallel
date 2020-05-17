// solopointer1202@gmail.com
#pragma once
#include "log.h"

namespace galois::gparallel {

std::string demangle(const char* name);

template <template<class> class M> class input; 
template <template<class> class M> class output;
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

template <class A, class NT>
void record_input_output(io_description & iodes) {
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

template <class STACK, class... MS> struct pop_and_process_M {};
template <class STACK, class... MS> struct push_next_M {};


// STACK: An abstract stack type, which story some info for Depth-First-Search
// D: Data type
// M: Data meta type
// MS_dep: The data meta types which depends it
// template_list: a list of any template
// meta_info_t: a special list for store <D, M, MS_dep ...>
//
// deducer<STACK, meta_info_t>
template <class STACK, class D, template <class> class M, template <class> class... MS_dep>
struct push_next_M<STACK, meta_info_t<D, M, MS_dep ...>> 
    : public pop_and_process_M<STACK, template_list<MS_dep...>> {
    template <class P>
    static void deduce(io_description & deps) {
#ifdef _DEBUG
        DEBUG("%s::deduce[normal]", demangle(typeid(push_next_M<STACK, meta_info_t<D, M, MS_dep ...>>).name()).c_str());
#endif
        pop_and_process_M<STACK, template_list<MS_dep...>>::template deduce<P>(deps);
    }
};

// end case
template <class STACK>
struct pop_and_process_M<STACK, template_list<>> : public STACK {
    template <class P>
    static void deduce(io_description & deps) {
#ifdef _DEBUG
        DEBUG("%s::deduce[call stack::deduce]", 
            demangle(typeid(pop_and_process_M<STACK, template_list<>>).name()).c_str());
#endif
        STACK::template deduce<P>(deps);
    }
};

// deducer<STACK, template_list>
template <class STACK, template <class> class meta_name, template <class> class...super_meta_names>
struct pop_and_process_M<STACK, template_list<meta_name, super_meta_names...>> 
    : public meta_name<push_next_M<pop_and_process_M<STACK, template_list<super_meta_names...>>, typename meta_name<none_type>::meta_info>> {
    template <class P>
    static void deduce(io_description & deps) {
#ifdef _DEBUG
        DEBUG("%s::deduce[record_input_output(%s)]", 
            demangle(typeid(pop_and_process_M<STACK, template_list<meta_name, super_meta_names...>>).name()).c_str(),
            demangle(typeid(meta_name<none_type>).name()).c_str()
        );
#endif
        record_input_output<input<meta_name>, P>(deps);
        push_next_M<pop_and_process_M<STACK, template_list<super_meta_names...>>, typename meta_name<none_type>::meta_info>::template deduce<P>(deps);
    }
};

// start
template <class T> struct depth_first_search_of_meta {};
template <class meta_storage_t, template <class> class meta_name, template <class> class... super_meta_names>
struct depth_first_search_of_meta< meta_info_t<meta_storage_t, meta_name, super_meta_names...> > 
    : public meta_name<pop_and_process_M<storage_reference<meta_storage_t>, template_list<super_meta_names...>>> {
    depth_first_search_of_meta(meta_storage_t* data) {
        this->reset(data); 
    }
    template <class P>
    static void deduce(io_description & deps) {
#ifdef _DEBUG
        DEBUG("%s::deduce[start]", demangle(typeid(depth_first_search_of_meta< meta_info_t<meta_storage_t, meta_name, super_meta_names...>>).name()).c_str());
#endif
        pop_and_process_M<storage_reference<meta_storage_t>, template_list<super_meta_names...>>::template deduce<P>(deps);
    };
};

template <template<class> class M>
class input {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::meta_storage_t meta_storage_t;
    typedef depth_first_search_of_meta<meta_info> meta_imp_t;
    input(meta_storage_t* data) : _m(data) {}
    meta_imp_t* operator-> () {
        return &_m;
    }
private:
    meta_imp_t _m;
};

template <template<class> class M>
class output {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::meta_storage_t meta_storage_t;
    typedef depth_first_search_of_meta<meta_info> meta_imp_t;
    output(meta_storage_t* data) : _m(data) {}
    meta_imp_t* operator->() {
        return &_m;
    }
private:
    meta_imp_t _m;
};

}