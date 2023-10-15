#ifndef __n_collection_hpp__
#define __n_collection_hpp__

#include <core/utils.hpp>

template <typename T>
concept collection_context = requires {
  typename T::type;
  typename T::position;
};

template <typename C>
concept collection =
    collection_context<C> and
    requires(C c, const C cc, typename C::type t, typename C::position p) {
      { cc.has(p) } -> same_as<bool>;
      { cc.at(p) } -> same_as_declined<typename C::type>;
      { cc.empty() } -> same_as<bool>;
      { cc.len() } -> same_as<size_t>;
      { c.add(t) } -> same_as<bool>;
      { c.modify(p, t) } -> same_as<bool>;
      { c.remove(p) } -> same_as<bool>;
      { c.clear() } -> same_as<void>;
    };

template <typename I>
concept iterator = collection_context<I> and requires(I i, const I ci) {
  { ci.has() } -> same_as<bool>;
  { i.next() } -> same_as<void>;
  { i.get() } -> same_as_declined<typename I::type>;
  { ci.pos() } -> same_as<typename I::position>;
};

template <typename I>
concept char_iterator = iterator<I> and character<typename I::type>;

template <typename C>
class index_forward_iterator {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  position _pos;
  C& _col;

 public:
  constexpr index_forward_iterator(C& v) : _pos(0), _col(v) {}

 public:
  constexpr bool has() const { return _col.has(_pos); }
  constexpr void next() { _pos += 1; }
  constexpr auto get() -> decltype(auto) { return _col.at(_pos); }
  constexpr position pos() const { return _pos; }
};

template <typename C>
class index_backward_iterator {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  position _pos;
  C& _col;

 public:
  constexpr index_backward_iterator(C& v)
      : _pos(v.len() == 0 ? 0 : v.len() - 1), _col(v) {}

 public:
  constexpr bool has() const { return _col.has(_pos); }
  constexpr void next() { _pos -= 1; }
  constexpr auto get() -> decltype(auto) { return _col.at(_pos); }
  constexpr position pos() const { return _pos; }
};

template <iterator I>
class limit_iterator {
 public:
  using type = typename I::type;
  using position = typename I::position;

 private:
  I _iter;
  size_t _limit;
  size_t _cur;

 public:
  constexpr limit_iterator(I i, size_t l) : _iter(i), _limit(l), _cur(0) {}

 public:
  constexpr bool has() const { return _cur != _limit and _iter.has(); }
  constexpr void next() {
    if (_cur != _limit) {
      _iter.next();
      _cur += 1;
    }
  }
  constexpr auto get() -> decltype(auto) { return _iter.get(); }
  constexpr position pos() const { return _iter.pos(); }

 public:
  constexpr size_t limit() const { return _limit; }
  constexpr size_t current() const { return _cur; }
  constexpr size_t tail() const { return _limit - _cur; }
};

template <iterator I>
constexpr auto find(I i, const typename I::type& t) -> typename I::position {
  while (i.has()) {
    if (i.get() == t) {
      break;
    }

    i.next();
  }

  return i.pos();
}

template <iterator I>
constexpr size_t count(I i) {
  size_t cnt = 0;

  while (i.has()) {
    cnt += 1;
    i.next();
  }

  return cnt;
}

template <iterator I>
constexpr size_t count(I i, const typename I::type& t) {
  size_t cnt = 0;

  while (i.has()) {
    if (i.get() == t) {
      cnt += 1;
    }

    i.next();
  }

  return cnt;
}

template <iterator I>
constexpr size_t count(I i, auto&& pred) {
  size_t cnt = 0;

  while (i.has()) {
    if (pred(i.get())) {
      cnt += 1;
    }

    i.next();
  }

  return cnt;
}

template <iterator I>
constexpr void transform(I i, auto&& unary) {
  while (i.has()) {
    auto& t = i.get();
    t = unary(t);
    i.next();
  }
}

template <iterator I, collection C>
constexpr void transform(I i, auto&& unary, C& c) {
  while (i.has()) {
    c.add(i.get());
    i.next();
  }
}

template <iterator I>
constexpr size_t replace(I i,
                         const typename I::type& old,
                         const typename I::type& neo) {
  size_t cnt = 0;

  while (i.has()) {
    auto& t = i.get();

    if (t == old) {
      t = neo;
      ++cnt;
    }

    i.next();
  }

  return cnt;
}

template <iterator I, collection C>
constexpr void replace(I i,
                       const typename I::type& old,
                       const typename I::type& neo,
                       C& c) {
  while (i.has()) {
    auto& t = i.get();
    c.add(t == old ? neo : t);
    i.next();
  }
}

template <iterator I>
constexpr bool all_of(I i, const auto& pred) {
  while (i.has()) {
    if (not pred(i.get())) {
      return false;
    }

    i.next();
  }

  return true;
}

template <iterator I>
constexpr bool any_of(I i, const auto& pred) {
  while (i.has()) {
    if (pred(i.get())) {
      return true;
    }

    i.next();
  }

  return false;
}

template <iterator I>
constexpr bool none_of(I i, const auto& pred) {
  while (i.has()) {
    if (pred(i.get())) {
      return false;
    }

    i.next();
  }

  return true;
}

template <iterator I, collection C>
constexpr void copy(I i, C& c) {
  while (i.has()) {
    c.add(i.get());
    i.next();
  }
}

template <iterator I, collection C>
constexpr void move(I i, C&& c) {
  while (i.has()) {
    c.add(move(i.get()));
    i.next();
  }
}

template <iterator I1, iterator I2>
constexpr bool equals(I1 i1, I2 i2) {
  while (i1.has() and i2.has()) {
    if (i1.get() != i2.get()) {
      break;
    }

    i1.next();
    i2.next();
  }

  return not i1.has() and not i2.has();
}

template <iterator I1, iterator I2>
constexpr bool starts_with(I1 i1, I2 i2) {
  while (i1.has() and i2.has()) {
    if (i1.get() != i2.get()) {
      break;
    }

    i1.next();
    i2.next();
  }

  return not i2.has();
}

#endif
