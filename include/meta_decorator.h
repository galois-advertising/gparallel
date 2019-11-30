
#pragma once

namespace galois::gparallel {
template <template<class> class M>
class input {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::data_meta_type data_meta_type;
    //typedef input_meta_imp<meta_info> input_imp;
    input(const data_meta_type* data) : _m(const_cast<data_meta_type*> (data)) {}
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
    typedef typename M<none_type>::data_meta_type data_meta_type;
    typedef M<data_wrapper<data_meta_type>> * meta_imp_type;

    struct output_storage_t : public M<data_wrapper<data_meta_type>> {
        output_storage_t (data_meta_type * data) { this->reset(data); }
    };

    output(meta_imp_type v) : _v(v) {}
    meta_imp_type _v;
};

template <template<class> class M>
class produce {
public:
    typedef typename M<none_type>::meta_info meta_info;
    typedef typename M<none_type>::data_meta_type data_meta_type;
};

}