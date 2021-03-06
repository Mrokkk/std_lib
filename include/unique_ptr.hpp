#pragma once

#include "move.hpp"
#include "spinlock.hpp"

namespace yacppl {

template<typename Type>
class unique_ptr final {

    using Reference = Type &;
    using Pointer = Type *;

    Pointer ptr_ = nullptr;
    static spinlock spinlock_;

    void release() {
        if (ptr_) {
            delete ptr_;
        }
        ptr_ = nullptr;
    }

public:

    constexpr unique_ptr() = default;

    constexpr unique_ptr(Pointer ptr) : ptr_(ptr) {
    }

    unique_ptr(const unique_ptr &) = delete;

    unique_ptr(unique_ptr &) = delete;

    unique_ptr(unique_ptr &&other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    ~unique_ptr() {
        auto _ = make_scoped_lock(spinlock_);
        release();
    }

    unique_ptr &operator=(unique_ptr &&other) {
        auto _ = make_scoped_lock(spinlock_);
        release();
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
        return *this;
    }

    template <typename T>
    void reset(T v) {
        operator=(v);
    }

    Pointer get() const {
        return ptr_;
    }

    Reference operator*() const {
        return *ptr_;
    }

    Pointer operator->() const {
        return ptr_;
    }

    operator Pointer() const {
        return ptr_;
    }

};

template <typename T> spinlock unique_ptr<T>::spinlock_;

template<typename Type>
inline unique_ptr<Type> make_unique() {
    return unique_ptr<Type>(new Type());
}

template<typename Type>
inline unique_ptr<Type> make_unique(Type &&a) {
    return unique_ptr<Type>(new Type(forward<Type>(a)));
}

template<typename Type, typename... Args>
unique_ptr<Type> make_unique(Args&&... args) {
    return unique_ptr<Type>(new Type(forward<Args>(args)...));
}

} // namespace yacppl

