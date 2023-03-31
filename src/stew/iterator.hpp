#ifndef __steward_iterator_hpp__
#define __steward_iterator_hpp__

#include <assert.h>

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
