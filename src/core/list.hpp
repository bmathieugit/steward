#ifndef __stew_core_list_hpp__
#define __stew_core_list_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/vector.hpp>

template <typename T>
class list {
 public:
  using type = T;
  using reference = T&;
  using const_reference = const T&;
  using move_reference = T&&;
  using pointer = T*;
  using const_pointer = const T*;

 private:
  vector<T> _data;
  vector<size_t> _index;

 public:
  constexpr ~list() = default;

  constexpr list() = default;

  constexpr list(size_t max) : _data(max), _index(max) {}

  constexpr list(const list&) = default;

  constexpr list(list&&) = default;

  constexpr list& operator=(const list&) = default;

  constexpr list& operator=(list&&) = default;

  constexpr auto len() const { return _data.len(); }

  constexpr auto empty() const { return _data.empty(); }

  constexpr auto max() const { return _data.max(); }

  constexpr auto full() const { return _data.full(); }

  constexpr bool has(size_t p) const { return _data.has(p); }

  constexpr void clear() {
    _data.clear();
    _index.clear();
  }

  constexpr reference at(size_t p) { return _data.at(_index.at(p)); }

  constexpr const_reference at(size_t p) const {
    return _data.at(_index.at(p));
  }

  template <convertible_to<T> U>
  constexpr void add(U&& u, size_t p) {
    _data.add(relay<U>(u));
    _index.add(_data.len() - 1, p);
  }

  template <convertible_to<T> U>
  constexpr void add(U&& u) {
    add(relay<U>(u), _data.len());
  }

  constexpr void exchange(size_t p1, size_t p2) { _index.exchange(p1, p2); }

  template <convertible_to<T> U>
  constexpr void modify(U&& u, size_t p) {
    _data.modify(relay<U>(u), _index.at(p));
  }

  constexpr void remove(size_t p) {
    _data.remove(_index.at(p));
    _index.remove(p);
  }
};


#endif
