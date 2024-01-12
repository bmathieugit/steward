#ifndef __steward_core_span_hpp__
#define __steward_core_span_hpp__

#include <core/core.hpp>
#include <core/exception.hpp>
#include <core/hash.hpp>

template <typename T>
class span {
 private:
  T* _begin = nullptr;
  size_t _len = 0;

 public:
  constexpr ~span() = default;

  constexpr span() = default;

  constexpr span(T* begin, T* end) : _begin(begin), _len(end - begin) {}

  constexpr span(T* begin, size_t len) : _begin(begin), _len(len) {}

  constexpr span(const span&) = default;

  constexpr span(span&&) = default;

  constexpr span& operator=(const span&) = default;

  constexpr span& operator=(span&&) = default;

  constexpr size_t len() const { return _len; }

  constexpr auto empty() const { return _len == 0; }

  constexpr bool has(size_t p) const { return p < _len; }

  constexpr T* data()
    requires non_const<T>
  {
    return _begin;
  }

  constexpr const T* data() const { return _begin; }

  constexpr T& at(size_t p)
    requires non_const<T>
  {
    if (p < _len) {
      return _begin[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr const T& at(size_t p) const {
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
class span_iterator {
 public:
  using type = T;

 private:
  T* _begin;
  T* _sentinel;

 public:
  constexpr span_iterator(span<T>& a)
      : _begin(a.data()), _sentinel(a.data() + a.len()) {}

 public:
  constexpr bool has_next() const { return _begin != _sentinel; }

  constexpr T& next() { return *(_begin++); }
};

template <typename T>
constexpr auto iter(span<T>& a) {
  return span_iterator(a);
}

template <typename T>
class span_const_iterator {
 public:
  using type = const T;

 private:
  const T* _begin;
  const T* _sentinel;

 public:
  constexpr span_const_iterator(const span<T>& a)
      : _begin(a.data()), _sentinel(a.data() + a.len()) {}

 public:
  constexpr bool has_next() const { return _begin != _sentinel; }

  constexpr const T& next() { return *(_begin++); }
};

template <typename T>
constexpr auto iter(const span<T>& a) {
  return span_const_iterator(a);
}

template <size_t N, typename T>
constexpr uof<N> to_hash(const span<T>& s, uof<N> h = fnvoffset<N>) {
  return to_hash<N>(iter(s));
}

#endif