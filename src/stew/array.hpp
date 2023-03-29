#ifndef __steward_array_hpp__
#define __steward_array_hpp__

#include <assert.h>

#include <stew/meta.hpp>

namespace stew {

template <typename T>
class array_iterator {
 private:
  T *_current = nullptr;
  T *_end = nullptr;

 public:
  array_iterator(T *current, T *end) : _current(current), _end(end) {}

 public:
  constexpr bool has_next() const { return _current != _end; }

  constexpr T &next() { return (*_current++); }
};

template <typename T>
class array_citerator {
 private:
  const T *_current = nullptr;
  const T *_end = nullptr;

 public:
  array_citerator(const T *current, const T *end)
      : _current(current), _end(end) {}

 public:
  constexpr bool has_next() const { return _current != _end; }

  constexpr T &next() { return (*_current++); }
};

template <class T, size_t N>
struct array {
  T _data[N];

 public:
  constexpr auto size() const { return N; }

  constexpr auto iter() { return array_iterator<T>(_data, _data + N); }

  constexpr auto iter() const { return array_iterator<T>(_data, _data + N); }

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

}  // namespace stew

#endif
