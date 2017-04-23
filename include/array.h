#pragma once

#include <cstddef>
#include "iterator.h"
#include "initializer_list.h"

namespace yacppl {

template<typename Type, size_t _size = 0>
class array {

    template <bool is_const>
    using detail_iterator = pointer_iterator<Type, is_const>;

    Type array_[_size];

public:

    using iterator = detail_iterator<false>;
    using const_iterator = detail_iterator<true>;

    array() = default;

    array(std::initializer_list<Type> list) {
        unsigned i = 0;
        for (auto v : list)
            array_[i++] = v;
    }

    size_t size() const {
        return _size;
    }

    Type &operator[](int index) {
        return array_[index];
    }

    iterator begin() {
        return iterator(array_);
    }

    const_iterator cbegin() const {
        return const_iterator(array_);
    }

    iterator end() {
        return iterator(array_ + _size);
    }

    const_iterator cend() const {
        return const_iterator(array_ + _size);
    }

    Type *data() {
        return array_;
    }

    const Type *data() const {
        return array_;
    }

};

} // namespace yacppl

