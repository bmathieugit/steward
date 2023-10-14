#ifndef __steward_algorithm_hpp__
#define __steward_algorithm_hpp__

#include <stew/meta.hpp>
#include <stew/meta/container.hpp>
#include <stew/meta/iterator.hpp>
#include <stew/utils.hpp>

namespace stew {

template <iterator I, typename F>
constexpr void for_each(I i, F &&f) {
  while (i.has_next()) {
    relay<F>(f)(i.next());
  }
}

template <iterator I0, iterator I1, typename F>
constexpr void for_each_joined(I0 i0, I1 i1, F &&f) {
  while (i0.has_next() && i1.has_next()) {
    relay<F>(f)(i0.next(), i1.next());
  }
}

template <iterator I, typename P>
constexpr size_t find(I i, P &&pred) {
  size_t idx = size_t(-1);
  bool fnd = false;

  while (i.has_next()) {
    ++idx;

    if (relay<P>(pred)(i.next())) {
      fnd = true;
      break;
    }
  }

  return fnd ? idx : size_t(-1);
}

template <iterator I, typename T>
constexpr size_t find(I i, const T &t) {
  size_t idx = size_t(-1);
  bool fnd = false;

  while (i.has_next()) {
    ++idx;

    if (i.next() == t) {
      fnd = true;
      break;
    }
  }

  return fnd ? idx : size_t(-1);
}

template <iterator I0, iterator I1>
constexpr bool equals(I0 i0, I1 i1) {
  while (i0.has_next() && i1.has_next() && i0.next() == i1.next())
    ;

  return !i0.has_next() && !i1.has_next();
}

template <iterator I0, iterator I1>
constexpr bool starts_with(I0 i0, I1 i1) {
  while (i0.has_next() && i1.has_next() && i0.next() == i1.next())
    ;

  return !i1.has_next();
}

template <iterator I, typename T>
constexpr size_t count(I i, const T &t) {
  size_t c = 0;

  for_each(i, [&c, &t](auto &&item) {
    if (item == t) ++c;
  });

  return c;
}

template <iterator I, typename P>
constexpr size_t count(I i, P &&pred)
  requires predicate<decltype(i.next())>
{
  size_t c = 0;

  for_each(i, [&c, &pred](auto &&item) {
    if (relay<P>(pred)(item)) ++c;
  });

  return c;
}

template <iterator I, typename P>
constexpr bool all_of(I i, P &&pred)
  requires predicate<decltype(i.next())>
{
  bool res = true;

  for_each(i, [&res, &pred](auto &&item) {
    if (!relay<P>(pred)(item)) {
      res = false;
      return;
    }
  });

  return res;
}

template <iterator I, typename P>
constexpr bool any_of(I i, P &&pred)
  requires predicate<decltype(i.next())>
{
  bool res = false;

  for_each(i, [&res, &pred](auto &&item) {
    if (relay<P>(pred)(item)) {
      res = true;
      return;
    }
  });

  return res;
}

template <iterator I, typename P>
constexpr bool none_of(I i, P &&pred)
  requires predicate<decltype(i.next())>
{
  bool res = true;

  for_each(i, [&res, &pred](auto &&item) {
    if (relay<P>(pred)(item)) {
      res = false;
      return;
    }
  });

  return res;
}

template <iterator I, typename T>
constexpr bool contains(I i, const T &t) {
  return any_of(i, [&t](auto &&item) { return item == t; });
}

template <iterator I0, iterator I1>
constexpr I0 skip(I0 i0, I1 i1) {
  if (starts_with(i0, i1)) {
    while (i0.has_next() && i1.has_next()) {
      i0.next();
      i1.next();
    }
  }

  return i0;
}

template <iterator I, typename C>
constexpr void copy(I i, C &c)
  requires push_container<C, decltype(i.next())>
{
  for_each(i, [&c](auto &&item) { c.push(item); });
}

template <iterator I, typename C>
constexpr void transfer(I i, C &c)
  requires push_container<C, decltype(i.next())>
{
  for_each(i, [&c](auto &&item) { c.push(transfer(item)); });
}
}  // namespace stew

#endif
