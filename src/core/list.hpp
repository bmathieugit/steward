#ifndef __stew_core_list_hpp__
#define __stew_core_list_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/vector.hpp>

template <typename T>
class list {
 public:
  using type = T;

 private:
  vector<size_t> _index;
  vector<T> _data;

 public:
  constexpr ~list() = default;

  constexpr list() = default;

  constexpr list(size_t max) : _index(max), _data(max) {}

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

  constexpr T& at(size_t p) { return _data.at(_index.at(p)); }

  constexpr const T& at(size_t p) const { return _data.at(_index.at(p)); }

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

template <typename T>
class list_iterator {
 public:
  using type = T;

 private:
  size_t _index;
  list<T>& _list;

 public:
  constexpr list_iterator(list<T>& l, size_t index = 0)
      : _index(index), _list(l) {}

 public:
  constexpr bool has_next() const { return _index != _list.len(); }

  constexpr T& next() { return _list.at(_index++); }

  constexpr size_t distance() const { return _list.len() - _index; }
};

template <typename T>
constexpr auto iter(list<T>& l) {
  return list_iterator<T>(l);
}

template <typename T>
class list_const_iterator {
 public:
  using type = T;

 private:
  size_t _index;
  const list<T>& _list;

 public:
  constexpr list_const_iterator(const list<T>& l, size_t index = 0)
      : _index(index), _list(l) {}

 public:
  constexpr bool has_next() const { return _index != _list.len(); }

  constexpr const T& next() { return _list.at(_index++); }

  constexpr size_t distance() const { return _list.len() - _index; }
};

template <typename T>
constexpr auto iter(const list<T>& l) {
  return list_const_iterator<T>(l);
}

#endif
