#include <iostream>
#include <vector>
#include "gparallel.h"
using namespace galois::gparallel;

class advertisement {
public:
    advertisement(uint64_t _id) : id(_id) {}
    uint64_t id;
    double ctr;
    double cpm;
};

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
        return this->data->advs_original;
    }
};

DECL_META(ctr, thread_data, original) {
    ctr_response_t & mutable_ctr_data() {
        return this->data->ctr_data;
    }
};

DECL_META(cpm, thread_data, original) {
    cpm_response_t & mutable_cpm_data() {
        return this->data->cpm_data;
    }
};

DECL_META(original_with_ctr_cpm, thread_data, original) {};

DECL_META(ctr_ordered_advlist, thread_data) {
    advlist_t & mutable_advs_ctr_ordered() {
        return this->data->advs_ctr_ordered;
    }
};

DECL_META(cpm_ordered_advlist, thread_data) {
    advlist_t & mutable_advs_cpm_ordered() {
        return this->data->advs_cpm_ordered;
    }
};


struct get_ctr_node {
    static void process(input<original> ori, output<ctr> ctr) {
        INFO("[gparallel] get_ctr_node", "");
    }
};

struct get_cpm_node {
    static void process(input<original> ori, output<cpm> cpm) {
        INFO("[gparallel] get_cpm_node", "");

    }
};

struct fill_node {
    static void process(input<ctr> ctr, input<cpm> cpm, input<original> ori, 
        output<original_with_ctr_cpm> ori_ctr_cpm) {
        INFO("[gparallel] fill_node", "");
    }
};

struct gen_ctr_node {
    static void process(input<original_with_ctr_cpm> ori_ctr_cpm, 
        output<ctr_ordered_advlist> ctr_ordered) {
        INFO("[gparallel] gen_ctr_node", "");

    }
};

struct gen_cpm_node {
    static void process(input<original_with_ctr_cpm> ori_ctr_cpm, 
        output<cpm_ordered_advlist> cpm_ordered) {
        INFO("[gparallel] gen_cpm_node", "");
    }
};

struct end_node {
    static void process(input<ctr_ordered_advlist>, input<cpm_ordered_advlist>) {
        INFO("[gparallel] end_node", "");
    }
};

int main() {
    thread_data td;
    dag_schema<thread_data> nodes;
    register_node<thread_data, get_ctr_node, get_cpm_node>::reg(nodes);
    register_node<thread_data, fill_node>::reg(nodes);
    register_node<thread_data, gen_ctr_node, gen_cpm_node, end_node>::reg(nodes);
    setup_dag_schema<thread_data>(nodes);
    if (auto tasks = topological_sort<thread_data>(nodes); tasks) {
        for (auto task : tasks.value()) {
            INFO("Execute[%s]", task->name().c_str());
            task->mutable_executor()(&td);
        }
    }
    return 0;
}