#ifndef __steward_iterator_hpp__
#define __steward_iterator_hpp__

#include <assert.h>
#include <stdio.h>

#include <stew/meta.hpp>
#include <stew/meta/iterator.hpp>
#include <stew/utils.hpp>

namespace stew {

template <typename T>
class pointer_iterator {
 private:
  T* _current = nullptr;
  T* _end = nullptr;

 public:
  constexpr pointer_iterator(T* current, T* end)
      : _current(current), _end(end) {}
  constexpr pointer_iterator(T* current, size_t n)
      : pointer_iterator(current, current + n) {}
  template <size_t N>
  constexpr pointer_iterator(T (&a)[N]) : _current(a), _end(a + N) {}

 public:
  constexpr bool has_next() const { return _current != _end; }
  constexpr T& next() { return *(_current++); }
  constexpr size_t size() const { return _end - _current; }
};

template <typename T>
class reverse_pointer_iterator {
 private:
  T* _current = nullptr;
  T* _end = nullptr;

 public:
  constexpr reverse_pointer_iterator(T* current, T* end)
      : _current(current), _end(end) {}
  constexpr reverse_pointer_iterator(T* current, size_t n)
      : _current(current), _end(current - n) {}

 public:
  constexpr bool has_next() const { return _current != _end; }
  constexpr T& next() { return *(_current--); }
  constexpr size_t size() const { return _current - _end; }
};

template <iterator I>
class counter_iterator {
 private:
  I _iter;
  size_t _count;

 public:
  constexpr counter_iterator(I iter, size_t count)
      : _iter(transfer(iter)), _count(count) {}

 public:
  constexpr bool has_next() const { return _iter.has_next() && _count != 0; }
  constexpr auto next() -> decltype(auto) {
    --_count;
    return _iter.next();
  }
  constexpr size_t size() const { return _count; }
};

template <iterator I, typename T>
class split_iterator {
 private:
  I _iter;
  T _delim;

 public:
  constexpr split_iterator(I iter, const T& delim)
      : _iter(iter), _delim(delim) {}
  constexpr split_iterator(I iter, T&& delim) : _iter(iter), _delim(delim) {}

 public:
  constexpr bool has_next() const { return _iter.has_next(); }

  constexpr auto next() {
    I start = _iter;
    size_t n = size_t(-1);

    while (_iter.has_next() && _iter.next() != _delim) {
      ++n;
    }

    return counter_iterator(start, _iter.has_next() ? n + 1 : size_t(-1));
  }
};

template <integral I>
class increment_iterator {
 private:
  I _min;
  I _max;

 public:
  constexpr increment_iterator(I min, I max) : _min(min), _max(max) {}

 public:
  constexpr bool has_next() const { return _min != _max; }
  constexpr I next() { return _min++; }
  constexpr size_t size() const { return _max - _min; }
};

template <integral I>
class decrement_iterator {
 private:
  I _max;
  I _min;

 public:
  constexpr decrement_iterator(I max, I min) : _max(max), _min(min) {}

 public:
  constexpr bool has_next() const { return _max != _min; }
  constexpr I next() { return _max--; }
  constexpr size_t size() const { return _max - _min; }
};

// generator/upto/downto
class generator_iterator {};

class push_iterator {};
class transfer_iterator {};

template <typename F, iterator I>
class map_iterator {
 private:
  I _iter;
  F _func;

 public:
  map_iterator(I&& iter, F&& func) {}

 public:
  bool has_next() { return _iter.has_next(); }

  auto next() -> decltype(auto) { return _func(_iter.next()); }
};

// class reverse_iterator ???
// filter
}  // namespace stew

#endif
