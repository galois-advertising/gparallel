#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <cxxabi.h>
#include "type_id.h"

namespace galois::gparallel 
{

struct none_type {};
struct auto_type {};


std::string demangle(const char* name);
enum LOG_LEVEL {
    FATAL,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE
};
void log(LOG_LEVEL loglevel, const char * fmt, ...);

enum parameter_type {
    NONE = 0, INPUT, OUTPUT, SOUT, PRODUCE, LIST_VIEW
};

enum class meta_level_t {
    NONE_LEVEL = 0, ITEM_LEVEL, QUERY_LEVEL
};

struct io_item {
    int id;
    meta_level_t meta_level;
    parameter_type type;
};

typedef std::map<int, io_item> node_io_map;
typedef std::vector<io_item> node_io_vec;
struct io_description {
    node_io_map input;
    node_io_map output;
};
template <class D, template <class> class... MS> struct meta_info_list {};
template <template <class> class... TMPS> struct template_list {};
template <class... TS> struct type_list {};
std::string demangle(const char* name);

template <class T>
std::string type(const T& t) {
    return demangle(typeid(t).name());
}

template <class T>
class data_wrapper{
public:
    typedef T data_type;
    void reset(T * data) {
        _data = data;
    }
    template <class P>
    static void deduce(io_description&) {}
    const T * data() const {
        return _data;
    }
    T * data() {
        return _data;
    }
    T * _data;
};
template <template<class> class M>
class input {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::data_meta_type data_meta_type;
};

template <template<class> class M>
class output {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::data_meta_type data_meta_type;
    typedef M<data_wrapper<data_meta_type>> * meta_imp_type;

    output(meta_imp_type v) : _v(v) {}
    meta_imp_type _v;
};

template <template<class> class M>
class produce {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::data_meta_type data_meta_type;
};

template <class M>
struct parameter_traits {
};

template <template <class> class M>
struct parameter_traits< input<M> > {
    typedef type_id<typename M<none_type>::meta_info, none_type> node_meta_id;
    typedef typename M<none_type>::meta_info meta_info;
    static const parameter_type ptype = INPUT;
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
    static const parameter_type ptype = OUTPUT;
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
    static const parameter_type ptype = PRODUCE;
    static int id() {
        return node_meta_id::instance().id();
    }
    static const char* name() {
        return node_meta_id::instance().name();
    }
};

template <class A, class NT>
void push_io(io_description & iodes) {
    if (parameter_traits<A>::ptype == NONE) {
        return;
    }

    bool is_item = true;
    io_item link;
    link.id = parameter_traits<A>::id();
    link.type = parameter_traits<A>::ptype;
    link.meta_level = is_item ? ITEM_LEVEL : QUERY_LEVEL;

    if (link.type == INPUT) {
        iodes.input[link.id] = link;
    } else {
        iodes.output[link.id] = link;
    }
};

class node;
typedef void(*batch_function_type)(node&);
typedef void(*query_function_type)(node&);
typedef void(*end_function_type)(node&);

}