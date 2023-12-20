#ifndef __core_array_hpp__
#define __core_array_hpp__

#include <core/core.hpp>
#include <core/exception.hpp>

template <typename T, size_t N>
struct array {
 public:
  using type = T;
  
  T _data[N];

 public:
  constexpr auto len() const { return N; }

  constexpr auto max() const { return N; }

  constexpr bool has(size_t p) const { return p < N; }

  constexpr T* data() { return _data; }

  constexpr const T* data() const { return _data; }

  constexpr T& at(size_t p) {
    if (p < N) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr const T& at(size_t p) const {
    if (p < N) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr void exchange(size_t p1, size_t p2) {
    if (p1 >= N or p2 >= N) {
      throw out_of_range();
    }

    if (p1 != p2) {
      auto tmp1 = move(_data[p1]);
      _data[p1] = move(_data[p2]);
      _data[p2] = move(tmp1);
    }
  }

  template <convertible_to<T> U>
  constexpr void modify(U&& u, size_t p) {
    if (p >= N) {
      throw out_of_range();
    }

    _data[p] = relay<U>(u);
  }
};

template <typename T, size_t N>
constexpr auto begin(array<T, N>& a) {
  return a.data();
}

template <typename T, size_t N>
constexpr auto end(array<T, N>& a) {
  return a.data() + a.len();
}

template <typename T, size_t N>
constexpr auto begin(const array<T, N>& a) {
  return a.data();
}

template <typename T, size_t N>
constexpr auto end(const array<T, N>& a) {
  return a.data() + a.len();
}

#endif