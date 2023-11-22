#ifndef __n_algorithm_hpp__
#define __n_algorithm_hpp__

#include <core/core.hpp>

template <iterator I>
constexpr size_t count(I i) {
  size_t c = 0;
  while (i.has()) {
    i.next();
    c += 1;
  }
  return c;
}

template <iterator I>
constexpr size_t count(I i, const typename I::type& t) {
  size_t c = 0;

  while (i.has()) {
    if (t == i.next()) {
      c += 1;
    }
  }

  return c;
}

template <iterator I>
constexpr size_t count(const I& i, predicate<typename I::type> auto&& pred) {
  size_t c = 0;

  while (i.has()) {
    if (pred(i.next())) {
      c += 1;
    }
  }

  return c;
}

template <iterator I>
constexpr bool all_of(I i, predicate<typename I::type> auto& pred) {
  while (i.has()) {
    if (not pred(i.next())) {
      return false;
    }
  }

  return true;
}

template <iterator I>
constexpr bool any_of(I i, predicate<typename I::type> auto& pred) {
  while (i.has()) {
    if (pred(i.next())) {
      return true;
    }
  }

  return false;
}

template <iterator I>
constexpr bool none_of(I i, predicate<typename I::type> auto& pred) {
  while (i.has()) {
    if (pred(i.next())) {
      return false;
    }
  }

  return true;
}

template <iterator I, oterator O>
constexpr void copy(I i, O o) {
  while (i.has()) {
    o.add(i.next());
  }
}

template <iterator I, oterator O>
constexpr void move(I i, O o) {
  while (i.has()) {
    o.add(move(i.next()));
  }
}

template <iterator I1, iterator I2>
constexpr bool equals(I1 i1, I2 i2) {
  while (i1.has() and i2.has()) {
    if (i1.next() != i2.next()) {
      break;
    }
  }

  return not i1.has() and not i2.has();
}

template <iterator I1, iterator I2>
constexpr bool operator==(I1 i1, I2 i2) {
  return equals(i1, i2);
}

template <collection C1, collection C2>
constexpr bool operator==(const C1& c1, const C2& c2) {
  return equals(iter(c1), iter(c2));
}

template <iterator I1, iterator I2>
constexpr bool starts_with(I1 i1, I2 i2) {
  while (i1.has() and i2.has()) {
    if (i1.next() != i2.next()) {
      break;
    }
  }

  return not i2.has();
}

#endif
