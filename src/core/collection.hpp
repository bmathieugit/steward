#ifndef __n_collection_hpp__
#define __n_collection_hpp__

#include <core/concepts.hpp>
#include <core/utils.hpp>

template <typename C>
class index_forward_input_stream {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  C& _col;
  position _pos;

 public:
  constexpr index_forward_input_stream(C& c) : _col(c), _pos(0) {}

 public:
  constexpr bool has() const { return _col.has(_pos); }
  constexpr auto next() -> decltype(auto) { return _col.at(_pos++); }
};

template <typename C>
class index_forward_output_stream {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  C& _col;

 public:
  constexpr index_forward_output_stream(C& c) : _col(c) {}

 public:
  constexpr bool add(const type& t) { return _col.add(t); }
  constexpr bool add(type&& t) { return _col.add(move(t)); }
};

template <typename C>
class index_backward_input_stream {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  C& _col;
  position _pos;

 public:
  constexpr index_backward_input_stream(C& c) : _col(c), _pos(_col.len() - 1) {}

 public:
  constexpr bool has() const { return _col.has(_pos); }
  constexpr auto next() -> decltype(auto) { return _col.at(_pos--); }
};

template <input_stream I>
constexpr auto find(I i, const typename I::type& t) -> typename I::position {
  while (i.has()) {
    if (i.next() == t) {
      break;
    }
  }

  return i.pos();
}

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
constexpr size_t count(I i, auto&& pred) {
  size_t cnt = 0;

  while (i.has()) {
    if (pred(i.next())) {
      cnt += 1;
    }
  }

  return cnt;
}

template <input_stream I>
constexpr bool all_of(I i, const auto& pred) {
  while (i.has()) {
    if (not pred(i.next())) {
      return false;
    }
  }

  return true;
}

template <input_stream I>
constexpr bool any_of(I i, const auto& pred) {
  while (i.has()) {
    if (pred(i.next())) {
      return true;
    }
  }

  return false;
}

template <input_stream I>
constexpr bool none_of(I i, const auto& pred) {
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
