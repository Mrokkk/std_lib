#pragma once

namespace yacppl {

namespace detail {

template <typename T>
struct type {
    void visit() const {
    }
};

template <typename ...Types>
struct max_sizeof {
};

template <typename T>
struct max_sizeof<T> {
    static constexpr const auto value = sizeof(T);
};

template <typename T1, typename T2>
struct max_sizeof<T1, T2> {
    static constexpr const auto value = sizeof(T1) > sizeof(T2) ? sizeof(T1) : sizeof(T2);
};

template <typename T1, typename T2, typename ...Rest>
struct max_sizeof<T1, T2, Rest...> {
    static constexpr const auto value = max_sizeof<T1, T2>::value > max_sizeof<Rest...>::value ?
        max_sizeof<T1, T2>::value : max_sizeof<Rest...>::value;
};

} // namespace detail

template <typename ...Types>
struct variant : detail::type<Types>... {

    constexpr variant() : data_() {
    }

    template <typename T>
    explicit variant(const T &v) {
        set<T>(v);
    }

    template <typename T>
    void set(const T &value) {
        detail::type<T>::visit();
        new(data_) T(value);
    }

    template <typename T>
    T &get() {
        detail::type<T>::visit();
        return *reinterpret_cast<T *>(data_);
    }

    template <typename T>
    const T &get() const {
        detail::type<T>::visit();
        return *reinterpret_cast<const T *>(data_);
    }

    template <typename T>
    variant &operator=(const T &val) {
        set<T>(val);
        return *this;
    }

private:

    char data_[detail::max_sizeof<Types...>::value];

};

} // namespace yacppl

