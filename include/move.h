#pragma once

#include "types.h"

namespace yacppl {

template <typename Type>
inline constexpr Type &&forward(typename remove_reference<Type>::type &t) {
    return static_cast<Type &&>(t);
}

template <typename Type>
inline constexpr Type &&forward(typename remove_reference<Type>::type &&t) {
    return static_cast<Type &&>(t);
}

template<typename Type>
inline constexpr typename remove_reference<Type>::type &&move(Type &&t) {
    return static_cast<typename remove_reference<Type>::type &&>(t);
}

} // namespace yacppl

