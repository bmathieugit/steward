#ifndef __steward_algorithm_hpp__
#define __steward_algorithm_hpp__

#include <stew/meta/iterable.hpp>
#include <stew/meta/container.hpp>
#include <stew/meta.hpp>
#include <stew/utils.hpp>

namespace stew {

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
  auto ii = i.iter();

  while (ii.has_next()) {
    if (ii.next() == t) {
      ++c;
    }
  }

  return c;
}

template <iterable I, typename P>

constexpr size_t count(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  size_t c = 0;
  auto ii = i.iter();

  while (ii.has_next()) {
    if (relay<P>(pred)(ii.next())) {
      ++c;
    }
  }

  return c;
}

template <iterable I, typename T>
constexpr bool contains(const I &i, const T &t) {
  auto ii = i.iter();

  while (ii.has_next()) {
    if (ii.next() == t) {
      return true;
    }
  }

  return false;
}

template <iterable I, typename P>
constexpr bool all_of(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  auto ii = i.iter();

  while (ii.has_next()) {
    if (!relay<P>(pred)(ii.next())) {
      return false;
    }
  }

  return true;
}

template <iterable I, typename P>
constexpr bool any_of(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  auto ii = i.iter();

  while (ii.has_next()) {
    if (relay<P>(pred)(ii.next())) {
      return true;
    }
  }

  return false;
}

template <iterable I, typename P>
constexpr bool none_of(const I &i, P &&pred)
  requires predicate<decltype(i.iter().next())>
{
  auto ii = i.iter();

  while (ii.has_next()) {
    if (relay<P>(pred)(ii.next())) {
      return false;
    }
  }

  return true;
}

template <iterable I, typename C>
constexpr void copy(const I &i, C &c)
  requires push_container<C, decltype(i.iter().next())>
{
  auto ii = i.iter();

  while (ii.has_next()) {
    c.push(ii.next());
  }
}

template <pop_container C0, typename C1>
constexpr void transfer(C0 &&c0, C1 &c1)
  requires push_container<C1, decltype(*c0.pop())>
{
  decltype(c0.pop()) m;

  while ((m = c0.pop()).has()) {
    c1.push(transfer(*m));
  }
}
}  // namespace stew

#endif
