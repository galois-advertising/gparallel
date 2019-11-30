
#pragma once
#include "util.h"
#include "parameter_traits.h"
namespace galois::gparallel {
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
}