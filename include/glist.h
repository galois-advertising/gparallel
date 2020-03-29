// solopointer1202@gmail.com
#pragma once
#include <type_traits>

namespace galois::gparallel
{

template <class... TT>
struct grow {};

template <class T>
struct grow<T> : public T {
};

template <class T, class... TT>
struct grow<T, TT...> : public grow<TT...> {
    operator T() {
        return T(this->data());
    }
};

template <class... columns>
class glist{};

template <class first_column, class... rest_columns>
class glist<first_column, rest_columns...> {
public:
    typedef typename first_column::data_type data_type;
    class _data_container {
    public:
        auto data() {
            return _data;
        }
    //private:
        data_type * _data;
    };

    class iterator : public grow<first_column, rest_columns..., _data_container> {
    public:
        iterator(data_type * item) {
            this->_data = item;
        }

        bool operator!=(const iterator & other) {
            return this->_data != other._data;
        }

        iterator & operator++() {
            this->_data += 1;
            return *this;
        }

        iterator & operator*() {
            return *this;
        }
    };

    iterator begin() {
        return iterator(_items);
    }

    iterator end() {
        return iterator(_items + _count);
    }

    glist(data_type * items, int count) : _items(items), _count(count) {}

private:
    data_type * _items;
    int _count;
};

}
