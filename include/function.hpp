#pragma once

#include <cstddef>
#include "move.hpp"
#include "unique_ptr.hpp"
#include "type_traits.hpp"

namespace yacppl {

namespace detail {

template <typename R, typename ...Args>
class callable {
public:
    virtual ~callable() {}
    virtual R operator()(Args ...args) const = 0;
};

template <typename ClosureType, typename R, typename ...Args>
class closure: public callable<R, Args...> {

    const ClosureType func_;

public:

    closure(ClosureType handler) : func_(handler) {
    }

    ~closure() {
    }

    R operator()(Args ...args) const override {
        if (is_void<R>::value)
            func_(forward<Args>(args)...);
        else
            return func_(forward<Args>(args)...);
    }

    constexpr void *operator new(size_t, void *address) {
        return address;
    }

};

} // namespace detail

template <typename FunctionType>
class function {};

template <typename R, typename ...Args>
class function<R(Args...)> {

    const detail::callable<R, Args...> *func_wrapper_ = nullptr;
    char data_[2 * sizeof(void *)]; // FIXME: why?

public:

    function() = default;

    template<typename ClosureType>
    function(ClosureType function)
            : func_wrapper_(new (data_) detail::closure<ClosureType, R, Args...>(function)) {
    }

    function &operator=(nullptr_t) = delete;

    template<typename ClosureType>
    function &operator=(ClosureType function) {
        func_wrapper_ = new (data_) detail::closure<ClosureType, R, Args...>(function);
        return *this;
    }

    template <typename T = R>
    typename enable_if<is_void<T>::value, T>::type operator()(Args ...args) const {
        (*func_wrapper_)(forward<Args>(args)...);
    }

    template <typename T = R>
    typename enable_if<!is_void<T>::value, T>::type operator()(Args ...args) const {
        return (*func_wrapper_)(forward<Args>(args)...);
    }

    operator bool() const {
        return func_wrapper_ != nullptr;
    }

};

} // namespace yacppl

