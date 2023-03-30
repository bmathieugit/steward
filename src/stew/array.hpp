#ifndef __steward_array_hpp__
#define __steward_array_hpp__

#include <assert.h>

#include <stew/iterator.hpp>
#include <stew/meta.hpp>

namespace stew {

template <class T, size_t N>
struct array {
  T _data[N];

 public:
  constexpr auto size() const { return N; }
  constexpr auto iter() { return pointer_iterator<T>(_data, N); }
  constexpr auto iter() const { return pointer_iterator<const T>(_data, N); }

  constexpr T &operator[](size_t i) {
    assert(i < N);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < N);
    return _data[i];
  }
};

template <class T>
struct array<T, 0> {
  T *_data = nullptr;

 public:
  constexpr auto size() const { return 0; }
  constexpr auto iter() { return pointer_iterator<T>(_data, 0); }
  constexpr auto iter() const { return pointer_iterator<const T>(_data, 0); }

  constexpr T &operator[](size_t i) {
    assert(i < 0);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < 0);
    return _data[i];
  }
};

template <size_t I, typename T, size_t N>
constexpr auto get(array<T, N> &a) -> decltype(auto) {
  static_assert(I < N);
  return a[I];
}

template <size_t I, typename T, size_t N>
constexpr auto get(const array<T, N> &a) -> decltype(auto) {
  static_assert(I < N);
  return a[I];
}

template <size_t I, typename T, size_t N>
constexpr auto get(array<T, N> &&a) -> decltype(auto) {
  static_assert(I < N);
  return a[I];
}

template <size_t I, typename T, size_t N>
constexpr auto get(const array<T, N> &&a) -> decltype(auto) {
  static_assert(I < N);
  return a[I];
}

}  // namespace stew

#endif
