#ifndef __n_algorithm_hpp__
#define __n_algorithm_hpp__

#include <core/core.hpp>

template <iterator I>
constexpr size_t count(I b, I e) {
  if constexpr (distanciable<I>) {
    return e - b;
  }

  else {
    size_t cnt = 0;

    while (b != e) {
      ++b;
      ++cnt;
    }

    return cnt;
  }
}

template <iterator I>
constexpr size_t count(I b, I e, const iterator_type<I>& t) {
  size_t cnt = 0;

  while (b != e) {
    if (t == *b) {
      ++cnt;
    }

    ++b;
  }

  return cnt;
}

template <iterator I>
constexpr size_t count(I b, I e, predicate<iterator_type<I>> auto&& pred) {
  size_t cnt = 0;

  while (b != e) {
    if (pred(*b)) {
      ++cnt;
    }

    ++b;
  }

  return cnt;
}

template <iterator I>
constexpr I find(I b, I e, const iterator_type<I>& t) {
  while (b != e and *b != t) {
    ++b;
  }

  return b;
}

template <iterator I>
constexpr I find(I b, I e, predicate<iterator_type<I>> auto&& pred) {
  while (b != e and not pred(*b)) {
    ++b;
  }

  return b;
}

template <iterator I>
constexpr I find_not(I b, I e, predicate<iterator_type<I>> auto&& pred) {
  while (b != e and pred(*b)) {
    ++b;
  }

  return b;
}

template <iterator I>
constexpr bool all_of(I b, I e, predicate<iterator_type<I>> auto&& pred) {
  return find_not(b, e, pred) == e;
}

template <iterator I>
constexpr bool any_of(I b, I e, predicate<iterator_type<I>> auto&& pred) {
  return find(b, e, pred) != e;
}

template <iterator I>
constexpr bool none_of(I b, I e, predicate<iterator_type<I>> auto&& pred) {
  return find(b, e, pred) == e;
}

template <iterator I1, iterator I2>
constexpr bool starts_with(I1 b1, I1 e1, I2 b2, I2 e2) {
  while (b1 != e1 and b2 != e2 and *b1 == *b2) {
    ++b1;
    ++b2;
  }

  return b2 == e2;
}

template <iterator I1, iterator I2>
constexpr bool equals(I1 b1, I1 e1, I2 b2, I2 e2) {
  while (b1 != e1 and b2 != e2 and *b1 == *b2) {
    ++b1;
    ++b2;
  }

  return b1 == e1 and b2 == e2;
}

template <iterator I, oterator O>
constexpr O transform(I b,
                      I e,
                      O o,
                      unary<iterator_type<I>, iterator_type<O>> auto&& un) {
  while (b != e) {
    *(o++) = un(*(b++));
  }

  return o;
}

template <iterator I, oterator O>
constexpr O copy(I b, I e, O o) {
  while (b != e) {
    *(o++) = *(b++);
  }

  return o;
}

template <iterator I, oterator O>
constexpr O move(I b, I e, O o) {
  while (b != e) {
    *(o++) = move(*(b++));
  }

  return o;
}

#endif
