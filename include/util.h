// solopointer1202@gmail.com
#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>
#include <iostream>
#include <cxxabi.h>
#include "type_id.h"

namespace galois::gparallel {

struct none_type {};
struct auto_type {};


std::string demangle(const char* name);

enum class parameter_type {
    NONE = 0, INPUT, OUTPUT
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
template <class D, template <class> class... MS> struct meta_info_t {};
template <template <class> class... TMPS> struct template_list {};
template <class... TS> struct type_list {};

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
    T * mutable_data() {
        return _data;
    }
private:
    T * _data;
};


}