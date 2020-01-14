#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <cxxabi.h>
#include "type_id.h"
#include "varlist.h"

namespace galois::gparallel 
{

struct none_type {};
struct auto_type {};


std::string demangle(const char* name);

enum class parameter_type {
    NONE = 0, INPUT, OUTPUT, SOUT, PRODUCE, LIST_VIEW
};

enum class meta_level_t {
    NONE = 0, ITEM, QUERY
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
class storage_reference {
public:
    typedef T data_type;
    void reset(T * data) {
        _data = data;
    }
    template <class P>
    static void deduce(io_description&) {/*End of deduce recurs, do nothing.*/}
    const T * data() const {
        return _data;
    }
    T * data() {
        return _data;
    }
    T * _data;
};

class node;
typedef void(*batch_function_type)(node&);
typedef void(*query_function_type)(node&);
typedef void(*end_function_type)(node&);

//template <class P, class... AS>
//struct process_traits_imp{
    //typedef varlist<typename parameter_traits<AS>::LocalTp...> list;
//};
//template <class P, class F>
//struct ProcTraits {};
//
//template <class P, class R, class... AS>
//struct process_traits_helper<P, R(*)(AS...)> : public process_traits_imp<P, AS...> {};
//
//template <class P, class R, class...AS>
//struct process_traits_helper<P, R(P::*)(AS...)> : public process_traits_imp<P, AS...> {};
//
//template <class P>
//struct process_traits : public process_traits_helper<P, decltype(&P::process)> {};

}