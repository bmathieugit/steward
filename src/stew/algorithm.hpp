#ifndef __steward_algorithm_hpp__
#define __steward_algorithm_hpp__

#include <stew/meta.hpp>
#include <stew/meta/container.hpp>
#include <stew/meta/iterable.hpp>
#include <stew/utils.hpp>

namespace stew {

template <iterable I, typename F>
constexpr void for_each(I &&i, F &&f) {
  auto ii = relay<I>(i).iter();

  while (ii.has_next()) {
    relay<F>(f)(ii.next());
  }
}

template <iterable I0, iterable I1>
constexpr bool equals(const I0 &i0, const I1 &i1) {
  auto ii0 = i0.iter();
  auto ii1 = i1.iter();

  while (ii0.has_next() && ii1.has_next() && ii0.next() == ii1.next())
    ;

  return !ii0.has_next() && !ii1.has_next();
}

template <iterable I0, iterable I1>
constexpr bool starts_with(const I0 &i0, const I1 &i1) {
  auto ii0 = i0.iter();
  auto ii1 = i1.iter();

  while (ii0.has_next() && ii1.has_next() && ii0.next() == ii1.next())
    ;

  return !ii1.has_next();
}

template <iterable I, typename T>
constexpr bool starts_with(const I &i, const T &t) {
  auto ii = i.iter();
  return ii.has_next() && ii.next() == t;
}

template <iterable I, typename T>
constexpr size_t count(const I &i, const T &t) {
  size_t c = 0;

  for_each(i, [&c, &t](auto &&item) {
    if (item == t) ++c;
  });

  return c;
}

template <iterable I, typename P>

constexpr size_t count(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  size_t c = 0;

  for_each(i, [&c, &pred](auto &&item) {
    if (relay<P>(pred)(item)) ++c;
  });

  return c;
}

template <iterable I, typename P>
constexpr bool all_of(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  bool res = true;

  for_each(i, [&res, &pred](auto &&item) {
    if (!relay<P>(pred)(item)) res = false;
  });

  return res;
}

template <iterable I, typename P>
constexpr bool any_of(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  bool res = false;

  for_each(i, [&res, &pred](auto &&item) {
    if (relay<P>(pred)(item)) res = true;
  });

  return res;
}

template <iterable I, typename P>
constexpr bool none_of(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  bool res = true;

  for_each(i, [&res, &pred](auto &&item) {
    if (relay<P>(pred)(item)) res = false;
  });

  return res;
}

template <iterable I, typename T>
constexpr bool contains(const I &i, const T &t) {
  return any_of(i, [&t](auto&& item){return item == t;});
}

template <iterable I, typename C>
constexpr void copy(const I &i, C &c)
  requires push_container<C, decltype(i.iter().next())>
{
  for_each(i, [&c](auto &&item) { c.push(item); });
}

template <iterable I, typename C>
constexpr void transfer(I &i, C &c)
  requires push_container<C, decltype(i.iter().next())>
{
  for_each(i, [&c](auto &&item) { c.push(transfer(item)); });
}

template <typename C, typename... T>
constexpr void push(C &c, T &&...t)
  requires(push_container<C, T> && ...)
{
  (c.push(relay<T>(t)), ...);
}

}  // namespace stew

#endif
