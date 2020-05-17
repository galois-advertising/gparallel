// solopointer1202@gmail.com
#pragma once
#include <functional>
#include "invoke_ex.h"

namespace galois::gparallel {
template<class meta_storage_t>
using executable_t = std::function<void(meta_storage_t*)>;

template<class meta_storage_t, class process_t>
class executor {
private:
    static bool invoke(process_t fn, meta_storage_t* storage) {
        invoke_ex(fn, storage);
        return true;
    }
public:
    static executable_t<meta_storage_t> create(process_t process) {
        return std::bind(&invoke, process, std::placeholders::_1);
    }
};


};