#include <iostream>
#include <vector>
#include "meta.h"
using namespace galois::gparallel;

class advertisement {
public:
    advertisement(uint64_t _id) : id(_id) {}
    uint64_t id;
    double ctr;
    double cpm;
}

using advlist_t = std::vector<advertisement>;
using ctr_response_t = std::vector<double>;
using cpm_response_t = std::vector<double>;

class thread_data {
public:
    advlist_t advs_original;
    advlist_t advs_ctr_ordered;
    advlist_t advs_cpm_ordered;
    ctr_response_t ctr_data;
    cpm_response_t cpm_data;
};

DECL_META(original, thread_data) {
    advlist_t & mutable_advs_original() {
        return advs_original;
    }
};

DECL_META(ctr, thread_data, original) {
    ctr_response_t & mutable_ctr_data() {
        return ctr_data;
    }
};

DECL_META(cpm, thread_data, original) {
    cpm_response_t & mutable_cpm_data() {
        return cpm_data;
    }
};

DECL_META(original_with_ctr_cpm, thread_data, original) {};

DECL_META(ctr_ordered_advlist, thread_data) {
    advlist_t & mutable_advs_ctr_ordered() {
        return advs_ctr_ordered;
    }
};

DECL_META(cpm_ordered_advlist, thread_data) {
    advlist_t & mutable_advs_cpm_ordered() {
        return advs_cpm_ordered;
    }
};


struct get_ctr_node {
    void process(input<original> ori, output<ctr> ctr) {

    }
};

struct get_cpm_node {
    void process(input<original> ori, output<cpm> cpm) {

    }
};

struct fill_node {
    void process(input<ctr> ctr, input<cpm> cpm, input<original> ori, 
        output<original_with_ctr_cpm> ori_ctr_cpm) {

    }
};

struct gen_ctr_node {
    void process(input<original_with_ctr_cpm> ori_ctr_cpm, 
        output<ctr_ordered_advlist> ctr_ordered) {

    }
};

struct gen_cpm_node {
    void process(input<original_with_ctr_cpm> ori_ctr_cpm, 
        output<cpm_ordered_advlist> cpm_ordered) {
    }
};

struct end_node {
    void process(input<ctr_ordered_advlist>, input<cpm_ordered_advlist>) {

    }
};

int main() {
    dag_schema nodes;
    register_node<get_ctr_node, get_cpm_node>::reg(nodes);
    register_node<fill_node>::reg(nodes);
    register_node<gen_ctr_node, gen_cpm_node, end_node>::reg(nodes);
    return 0;
}