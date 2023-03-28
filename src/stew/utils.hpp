#ifndef __steward_utils_hpp__
#define __steward_utils_hpp__

#include <stew/meta.hpp>

namespace stew {
template <typename T>
constexpr T&& relay(rm_ref<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr add_rref<rm_ref<T>> transfer(T&& t) {
  return static_cast<add_rref<rm_ref<T>>>(t);
}

template <typename T>
concept native_number_reference = native_number<rm_cvref<T>>;

template <native_number_reference T>
constexpr auto transfer(T&& t) {
  if constexpr (native_number<T>) {
    return t;
  } else {
    rm_cvref<T> copy = t;
    t = 0;
    return copy;
  }
}

template <typename T>
concept pointer_reference = pointer_eq<rm_cvref<T>>;

template <pointer_reference T>
constexpr auto transfer(T&& p) {
  if constexpr (pointer_eq<T>) {
    return p;
  } else {
    rm_cvref<T> copy = p;
    p = nullptr;
    return copy;
  }
}

}  // namespace stew

#endif
