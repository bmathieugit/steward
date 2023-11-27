#ifndef __stew_core_iterator_hpp__
#define __stew_core_iterator_hpp__

#include <core/core.hpp>
#include <core/tuple.hpp>

// template <typename I0, iterator... IN>
//   requires same_all<typename I0::type, typename IN::type...>;
// class chain {
//  public:
//   using type = typename I0::type;

//  private:
//   tuple<IO, IN...> _iters;
//   size_t _rank = 0;

//  public:
//   constexpr chain(I0 i0, IN... in) : _iters(i0, in...) {}
//   constexpr bool has() {
//     return rank < (sizeof(IN) + 1) and get<_rank>(_iters).has();
//   }

//   constexpr type next() { return get<_rank>(_iters).next(); }
// };

template <iterator I1, iterator I2>
  requires same_as<typename I1::type, typename I2::type>
class chain {
 public:
  using type = typename I1::type;

 private:
  I1 _i1;
  I2 _i2;
  bool _first = true;

 public:
  constexpr chain(I1 i1, I2 i2) : _i1(i1), _i2(i2) {}
  constexpr bool has() { return _i1.has() ? _i1.has() : _i2.has(); }
  constexpr auto next() -> decltype(auto) {
    return _i1.has() ? _i1.next() : _i2.next();
  }
};

template <iterator I>
class copied {
 public:
  using type = typename I::type;

 private:
  I _iter;

 public:
  constexpr copied(I i) : _iter(i) {}
  constexpr bool has() { return _iter.has(); }
  constexpr auto next() { return _iter.next(); }
};

class chunk {};

template <iterator I>
class intersperse {
 public:
  using type = typename I::type;

 private:
  I _iter;
  type _interstice;
  bool _inter = true;

 public:
  constexpr intersperse(I i, const type& t) : _iter(i), _interstice(t) {}
  constexpr intersperse(I i, type&& t) : _iter(i), _interstice(move(t)) {}

  constexpr bool has() { return _iter.has(); }
  constexpr const auto& next() {
    return (_inter = not _inter) ? _interstice : _iter.next();
  }
};

template <iterator I>
class cycle {
 public:
  using type = typename I::type;

 private:
  I _save;
  I _iter;

 public:
  constexpr cycle(I i) : _save(i), _iter(i) {}
  constexpr bool has() { return _save.has(); }
  constexpr auto next() -> decltype(auto) {
    return _iter.has() ? _iter.next() : (_iter = _save).next();
  }
};

template <iterator I>
class enumerate {
 public:
  using type = tuple<size_t, typename I::type>;

 private:
  I _iter;
  size_t _idx = 0;

 public:
  constexpr enumerate(I i) : _iter(i) {}
  constexpr bool has() { return _iter.has(); }
  constexpr type next() { return tuple(_idx++, _iter.next()); }
};

class filter {};
class repeat {};
class skip {};
class take {};
class zip {};

#endif