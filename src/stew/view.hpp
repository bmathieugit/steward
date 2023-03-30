#ifndef __steward_view_hpp__
#define __steward_view_hpp__

#include <stew/iterator.hpp>
#include <stew/meta.hpp>

namespace stew {

template <iterator I>
class view {
 private:
  I _iter;

 public:
  constexpr ~view() = default;
  constexpr view(const I& iter) : _iter(iter) {}
  constexpr view(I&& iter) : _iter(transfer(iter)) {}

  // move
  constexpr view(view&&) = default;
  constexpr view& operator=(view&&) = default;

  // copy
  constexpr view(const view&) = default;
  constexpr view& operator=(const view&) = default;

 public:
  constexpr auto iter() { return _iter; };
};

template <sizeable_iterator I>
class sizeable_view : public view<I> {
 private:
  size_t _size = 0;

 public:
  constexpr ~sizeable_view() = default;
  constexpr sizeable_view(const I& iter) : view<I>(iter), _size(iter.size()) {}
  constexpr sizeable_view(I&& iter)
      : view<I>(transfer(iter)), _size(iter.size()) {}

  // move
  constexpr sizeable_view(sizeable_view&&) = default;
  constexpr sizeable_view& operator=(sizeable_view&&) = default;

  // copy
  constexpr sizeable_view(const sizeable_view&) = default;
  constexpr sizeable_view& operator=(const sizeable_view&) = default;

 public:
  constexpr size_t size() const { return _size; }
  constexpr bool empty() const { return _size == 0; }
};

template <typename T>
class pointer_view : public sizeable_view<pointer_iterator<T>> {
 public:
  constexpr ~pointer_view() = default;
  constexpr pointer_view(T* current, T* end)
      : sizeable_view<pointer_iterator<T>>(pointer_iterator<T>(current, end)) {}
  constexpr pointer_view(T* current, size_t n)
      : sizeable_view<pointer_iterator<T>>(pointer_iterator<T>(current, n)) {}

  // move
  constexpr pointer_view(pointer_view&&) = default;
  constexpr pointer_view& operator=(pointer_view&&) = default;

  // copy
  constexpr pointer_view(const pointer_view&) = default;
  constexpr pointer_view& operator=(const pointer_view&) = default;
};

}  // namespace stew

#endif
