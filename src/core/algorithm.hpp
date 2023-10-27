#ifndef __n_algorithm_hpp__
#define __n_algorithm_hpp__

#include <core/core.hpp>
#include <core/result.hpp>
/////////////////////////////
/// Collection algorithms ///
/////////////////////////////

template <collection C>
constexpr size_t count(const C& c) {
  return c.len();
}

template <collection C>
constexpr size_t count(const C& c, const typename C::type& t) {
  size_t cnt = 0;
  auto ord = c.ord();

  while (ord.has()) {
    if (c.at(ord.next()) == t) {
      ++cnt;
    };
  }

  return cnt;
}

template <collection C>
constexpr size_t count(const C& c, predicate<typename C::type> auto&& pred) {
  size_t cnt = 0;
  auto ord = c.ord();

  while (ord.has()) {
    if (pred(c.at(ord.next()))) {
      ++cnt;
    };
  }

  return cnt;
}

template <collection C>
constexpr maybe<typename C::position> find(const C& c,
                                           const typename C::type& t) {
  auto ord = c.ord();

  while (ord.has()) {
    auto p = ord.next();

    if (c.at(p) == t) {
      return p;
    };
  }

  return {};
}

template <collection C>
constexpr maybe<typename C::position> find(
    const C& c,
    predicate<typename C::type> auto&& pred) {
  auto ord = c.ord();

  while (ord.has()) {
    auto p = ord.next();

    if (pred(c.at(p))) {
      return p;
    };
  }

  return {};
}

template <collection C>
constexpr bool all_of(const C& c, predicate<typename C::type> auto& pred) {
  auto ord = c.ord();

  while (ord.has()) {
    if (not pred(c.at(ord.next()))) {
      return false;
    }
  }

  return true;
}

template <collection C>
constexpr bool any_of(const C& c, predicate<typename C::type> auto& pred) {
  auto ord = c.ord();

  while (ord.has()) {
    if (pred(c.at(ord.next()))) {
      return true;
    }
  }

  return false;
}

template <collection C>
constexpr bool none_of(const C& c, predicate<typename C::type> auto& pred) {
  auto ord = c.ord();

  while (ord.has()) {
    if (pred(c.at(ord.next()))) {
      return false;
    }
  }

  return true;
}

template <collection C, collection O>
constexpr void copy(const C& c, O& o) {
  auto ord = c.ord();

  while (ord.has()) {
    o.add(c.at(ord.next()));
  }
}

template <collection C, collection O>
constexpr void move(C&& c, O& o) {
  auto ord = c.ord();

  while (ord.has()) {
    o.add(move(c.at(ord.next())));
  }
}

template <collection C1, collection C2>
constexpr bool equals(const C1& c1, const C2& c2) {
  auto ord1 = c1.ord();
  auto ord2 = c2.ord();

  while (ord1.has() and ord2.has()) {
    if (c1.at(ord1.next()) != c2.at(ord2.next())) {
      break;
    }
  }

  return not ord1.has() and not ord2.has();
}

template <collection C1, collection C2>
constexpr bool starts_with(const C1& c1, const C2& c2) {
  auto ord1 = c1.ord();
  auto ord2 = c2.ord();

  while (ord1.has() and ord2.has()) {
    if (c1.at(ord1.next()) != c2.at(ord2.next())) {
      break;
    }
  }

  return not ord2.has();
}

template <collection C, input_stream IP>
  requires same_as<typename C::position, typename IP::type>
C& shuffle(C& c, IP ip) {
  auto ord = c.ord();

  while (ord.has() and ip.has()) {
    auto pc = ord.next();
    auto pip = ip.next();

    auto tmp = move(c.at(pc));
    c.modify(pc, move(c.at(pip)));
    c.modify(pip, move(tmp));
  }

  return c;
}

//////////////////////////
/// Streams algorithms ///
//////////////////////////

template <input_stream I>
constexpr size_t count(I i) {
  size_t cnt = 0;

  while (i.has()) {
    cnt += 1;
    i.next();
  }

  return cnt;
}

template <input_stream I>
constexpr size_t count(I i, const typename I::type& t) {
  size_t cnt = 0;

  while (i.has()) {
    if (i.next() == t) {
      cnt += 1;
    }
  }

  return cnt;
}

template <input_stream I>
constexpr size_t count(I i, predicate<typename I::type> auto&& pred) {
  size_t cnt = 0;

  while (i.has()) {
    if (pred(i.next())) {
      cnt += 1;
    }
  }

  return cnt;
}

template <input_stream I>
constexpr bool all_of(I i, predicate<typename I::type> auto& pred) {
  while (i.has()) {
    if (not pred(i.next())) {
      return false;
    }
  }

  return true;
}

template <input_stream I>
constexpr bool any_of(I i, predicate<typename I::type> auto& pred) {
  while (i.has()) {
    if (pred(i.next())) {
      return true;
    }
  }

  return false;
}

template <input_stream I>
constexpr bool none_of(I i, predicate<typename I::type> auto& pred) {
  while (i.has()) {
    if (pred(i.next())) {
      return false;
    }
  }

  return true;
}

template <input_stream I, output_stream O>
constexpr void copy(I i, O o) {
  while (i.has()) {
    o.add(i.next());
  }
}

template <input_stream I, output_stream O>
constexpr void move(I i, O o) {
  while (i.has()) {
    o.add(move(i.next()));
  }
}

template <input_stream I1, input_stream I2>
constexpr bool equals(I1 i1, I2 i2) {
  while (i1.has() and i2.has()) {
    if (i1.next() != i2.next()) {
      break;
    }
  }

  return not i1.has() and not i2.has();
}

template <input_stream I1, input_stream I2>
constexpr bool starts_with(I1 i1, I2 i2) {
  while (i1.has() and i2.has()) {
    if (i1.next() != i2.next()) {
      break;
    }
  }

  return not i2.has();
}

#endif
