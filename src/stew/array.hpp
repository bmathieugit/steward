#ifndef __steward_array_hpp__
#define __steward_array_hpp__

#include <stew/meta.hpp>
#include <assert.h>

namespace stew {

template <class T, size_t N>
struct array {
  T _data[N];

 public:
  constexpr auto size() const { return N; }

  constexpr auto begin() { return _data; }

  constexpr auto end() { return _data + N; }

  constexpr const auto begin() const { return _data; }

  constexpr const auto end() const { return _data + N; }

  constexpr T &operator[](size_t i) {
    assert(i < N);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < N);
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


}

#endif 
