#ifndef __steward_core_span_hpp__
#define __steward_core_span_hpp__

#include <core/core.hpp>
#include <core/exception.hpp>

template <typename T>
class span {
 public:
  using type = T;
  using reference = T&;
  using const_reference = const T&;
  using move_reference = T&&;
  using pointer = T*;
  using const_pointer = const T*;

 private:
  pointer _begin;
  size_t _len;

 public:
  constexpr ~span() = default;

  constexpr span() = default;

  constexpr span(pointer begin, pointer end)
      : _begin(begin), _len(end - begin) {}

  constexpr span(pointer begin, size_t len) : _begin(begin), _len(len) {}

  constexpr span(const span&) = default;

  constexpr span(span&&) = default;

  constexpr span& operator=(const span&) = default;

  constexpr span& operator=(span&&) = default;

  constexpr size_t len() const { return _len; }

  constexpr auto empty() const { return _len == 0; }

  constexpr bool has(size_t p) const { return p < _len; }

  constexpr pointer data()
    requires non_const<T>
  {
    return _begin;
  }

  constexpr const_pointer data() const { return _begin; }

  constexpr reference at(size_t p)
    requires non_const<T>
  {
    if (p < _len) {
      return _begin[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr const_reference at(size_t p) const {
    if (p < _len) {
      return _begin[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr void exchange(size_t p1, size_t p2)
    requires non_const<T>
  {
    if (p1 >= _len or p2 >= _len) {
      throw out_of_range();
    }

    if (p1 != p2) {
      auto tmp1 = move(_begin[p1]);
      _begin[p1] = move(_begin[p2]);
      _begin[p2] = move(tmp1);
    }
  }

  template <convertible_to<T> U>
  constexpr void modify(U&& u, size_t p)
    requires non_const<T>
  {
    if (p >= _len) {
      throw out_of_range();
    }

    _begin[p] = relay<U>(u);
  }
};

template <typename T>
constexpr auto begin(const span<T>& s) {
  return s.data();
}

template <typename T>
constexpr auto end(const span<T>& s) {
  return s.data() + s.len();
}

template <typename T>
constexpr auto begin(span<T>& s) {
  return s.data();
}

template <typename T>
constexpr auto end(span<T>& s) {
  return s.data() + s.len();
}

#endif