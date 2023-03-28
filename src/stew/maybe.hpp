#ifndef __steward_maybe_hpp__
#define __steward_maybe_hpp__

#include <stew/meta.hpp>
#include <stew/utils.hpp>

namespace stew {

struct maybe_empty {};

template <typename T>
class maybe {
 private:
  bool _has = false;

  union {
    T _t;
    maybe_empty _e;
  };

 public:
  constexpr ~maybe() {
    if (_has) {
      _t.~T();
    }
  }

  constexpr maybe() : _has(false), _e() {}

  constexpr maybe(const T &t) : _has(true), _t(t) {}
  constexpr maybe(T &&t) : _has(true), _t(transfer(t)) {}

  constexpr maybe(const maybe &o) : _has(o._has) {
    if (_has) {
      _t = o._t;
    }
  }

  constexpr maybe(maybe &&o) : _has(transfer(o._has)) {
    if (_has) {
      _t = transfer(o._t);
    }
  }

  constexpr maybe &operator=(const T &t) {
    _has = true;
    _t = t;
    return *this;
  }

  constexpr maybe &operator=(T &&t) {
    _has = true;
    _t = transfer(t);
    return *this;
  }

  constexpr maybe &operator=(maybe o) {
    _has = transfer(o._has);

    if (_has) {
      _t = transfer(o._t);
    }

    return *this;
  }

 public:
  constexpr operator T &() { return _t; }
  constexpr operator const T &() const { return _t; }
  constexpr T &operator*() { return _t; }
  constexpr const T &operator*() const { return _t; }
  constexpr bool has() const { return _has; }

 public:
  template <typename F>
  constexpr auto map(F &&f) & {
    using TR = T &;
    using U = decltype(relay<F>(f)(fakeval<TR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<TR>(_t))) : MU();
  }

  template <typename F>
  constexpr auto map(F &&f) const & {
    using CTR = const T &;
    using U = decltype(relay<F>(f)(fakeval<CTR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<CTR>(_t))) : MU();
  }

  template <typename F>
  constexpr auto map(F &&f) && {
    using TRR = T &&;
    using U = decltype(relay<F>(f)(fakeval<TRR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<TRR>(_t))) : MU();
  }

  template <typename F>
  constexpr auto map(F &&f) const && {
    using CTRR = const T &&;
    using U = decltype(relay<F>(f)(fakeval<CTRR>()));
    using MU = maybe<U>;

    return _has ? MU(relay<F>(f)(cast<CTRR>(_t))) : MU();
  }
};

}  // namespace stew

#endif
