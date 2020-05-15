
// solopointer1202@gmail.com
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include "util.h"
#include "type_id.h"

namespace galois::gparallel {

void operator += (id_set_t & i, const id_set_t & other) {
    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o;});
}

bool operator -= (id_set_t & i, const id_set_t & other) {
    id_set_t diff; 
    if (i.size() < other.size()) {
        for (auto id : i) {
            if (other.find(id) != other.end()) {
                diff.insert(id);
            }
        }
    } else {
        for (auto id : other) {
            if (i.find(id) != i.end()) {
                diff.insert(id);
            }
        }
    }
    for (auto id : diff) {
        i.erase(id);
    }
    return diff.size() != 0;
}

void operator += (id_set_t & i, const node_io_vec & other) {

    std::transform(other.cbegin(), other.cend(),std::inserter(i, i.end()), 
        [](auto o){return o.id;});
}

void operator += (node_io_vec & i, const node_io_vec & other) {
    i.insert(i.end(), other.begin(), other.end());
}

node_io_vec operator + (node_io_vec & a, const node_io_vec & b) {
    node_io_vec res;
    res += a;
    res += b;
    return std::move(res);
}
bool transitive_closure(topology_t& implies) {
    bool has_change = true;
    while (has_change) {
        has_change = false;
        // for every output
        for (auto & [output, input_set] : implies) {
            id_set_t sub;
            // See every input<> as output<> of other nodes,
            // and we find the input<> or 'other nodes',
            // these input<> were also 
            for (auto input_id : input_set) {
                sub += implies[input_id];
            }
            size_t prev_size = input_set.size();
            input_set += sub;
            if (input_set.size() > prev_size) {
                has_change = true;
            }
        }
    }
    return true;
}
}