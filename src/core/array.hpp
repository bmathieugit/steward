#ifndef __n_array_hpp__
#define __n_array_hpp__

#include <core/collection.hpp>
#include <core/result.hpp>
#include <core/utils.hpp>

template <typename T, size_t N>
class array {
 public:
  using type = T;
  using position = size_t;

 private:
  T _data[N];
  size_t _len = 0;

 public:
  constexpr ~array() = default;
  constexpr array() = default;

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return N; }
  constexpr auto full() const { return _len == N; }

 public:
  constexpr bool has(position p) const { return p < _len; }

  constexpr T& at(position p) { return _data[p]; }

  constexpr const T& at(position p) const { return _data[p]; }

  constexpr void clear() {
    for (size_t i = 0; i < _len; ++i) {
      _data[i] = T();
    }

    _len = 0;
  }

  constexpr bool add(const T& t) {
    if (not full()) {
      _data[_len++] = t;
      return true;
    }
    return false;
  }

  constexpr bool add(T&& t) {
    if (not full()) {
      _data[_len++] = move(t);
      return true;
    }
    return false;
  }

  constexpr bool modify(position p, const T& t) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = t;
      return true;
    }
  }

  constexpr bool modify(position p, T&& t) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = move(t);
      return true;
    }
  }

  constexpr bool remove(position p) {
    if (p >= _len) {
      return false;
    } else {
      for (size_t i = p; i < _len - 1; ++i) {
        _data[i] = _data[i + 1];
      }

      _data[_len - 1] = T();
      _len -= 1;

      return true;
    }
  }
};

template <typename T, size_t N>
constexpr auto istream(const array<T, N>& a) {
  return index_forward_input_stream(a);
}

template <typename T, size_t N>
constexpr auto ristream(const array<T, N>& a) {
  return index_backward_input_stream(a);
}

template <typename T, size_t N>
constexpr auto ostream(array<T, N>& a) {
  return index_forward_output_stream(a);
};

#endif
