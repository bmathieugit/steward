#ifndef __n_utils_hpp__
#define __n_utils_hpp__

using size_t = unsigned long;
using byte_t = unsigned char;

template <bool test, typename T, typename U>
struct __if_ {
  using type = U;
};

template <typename T, typename U>
struct __if_<true, T, U> {
  using type = T;
};

template <bool test, typename T, typename U>
using if_ = typename __if_<test, T, U>::type;


template <typename T>
struct __rm_ref {
  using type = T;
};

template <typename T>
struct __rm_ref<T&> {
  using type = T;
};

template <typename T>
struct __rm_ref<T&&> {
  using type = T;
};

template <typename T>
using rm_ref = typename __rm_ref<T>::type;

template <typename T>
struct __rm_const {
  using type = T;
};

template <typename T>
struct __rm_const<const T> {
  using type = T;
};

template <typename T>
using rm_const = typename __rm_const<T>::type;

template <typename T>
using rm_cref = rm_const<rm_ref<T>>;



template <typename T>
constexpr rm_ref<T>&& move(T&& t) {
  return static_cast<rm_ref<T>&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
void* operator new(size_t, T* ptr) {
  return ptr;
}

#endif
