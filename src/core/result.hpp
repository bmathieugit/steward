#ifndef __n_result_hpp__
#define __n_result_hpp__

#include <core/concepts.hpp>
#include <core/utils.hpp>

template <typename... TN>
constexpr auto sizest = sizest<TN...>;

template <>
constexpr auto sizest<> = 0;

template <typename T0>
constexpr auto sizest<T0> = sizeof(T0);

template <typename T0, typename... TN>
constexpr auto sizest<T0, TN...> = sizeof(T0) > sizest<TN...> ? sizeof(T0)
                                                              : sizest<TN...>;

template <typename U, typename T0, typename... T>
constexpr size_t indexof =
    same_as<U, T0> ? 0 : (indexof<U, T...> == -1 ? -1 : indexof<U, T...> + 1);

template <typename U, typename T0>
constexpr size_t indexof<U, T0> = same_as<U, T0> ? 0 : size_t(-1);

template <typename T0, typename... T>
class variant {
 private:
  char _data[sizest<T0, T...>];
  size_t _index;

 private:
  template <typename U0, typename... UN>
  constexpr void __destroy__() {
    if (_index == indexof<U0, T0, T...>) {
      reinterpret_cast<U0*>(_data)->~U0();
    } else if constexpr (sizeof...(UN) > 0) {
      __destroy__<UN...>();
    }
  }

  template <typename U0, typename... UN>
  constexpr void __copy__(const variant& v) {
    if (v._index == indexof<U0, T0, T...>) {
      _index = v._index;
      new (reinterpret_cast<U0*>(_data))
          U0(*reinterpret_cast<const U0*>(v._data));
    } else if constexpr (sizeof...(UN) > 0) {
      __copy__<UN...>(v);
    }
  }

  template <contains<T0, T...> U>
  constexpr void __copy__(const U& u) {
    _index = indexof<U, T0, T...>;
    new (reinterpret_cast<U*>(_data)) U(u);
  }

  template <typename U0, typename... UN>
  constexpr void __move__(variant&& v) {
    if (v._index == indexof<U0, T0, T...>) {
      _index = v._index;
      new (reinterpret_cast<U0*>(_data))
          U0(move(*reinterpret_cast<U0*>(v._data)));
    } else if constexpr (sizeof...(UN) > 0) {
      __move__<UN...>(move(v));
    }
  }

  template <contains<T0, T...> U>
  constexpr void __move__(U&& u) {
    _index = indexof<U, T0, T...>;
    new (reinterpret_cast<U*>(_data)) U(move(u));
  }

 public:
  constexpr ~variant() { __destroy__<T0, T...>(); }

  template <contains<T0, T...> U>
  constexpr variant(const U& u) {
    __copy__(u);
  }

  template <contains<T0, T...> U>
  constexpr variant(U&& u) {
    __move__(move(u));
  }

  constexpr variant(const variant& v) { __copy__<T0, T...>(v); }

  constexpr variant(variant&& v) { __move__<T0, T...>(v); }

  constexpr variant& operator=(const variant& v) {
    if (this != &v) {
      __destroy__<T0, T...>(v);
      __copy__<T0, T...>(v);
    }

    return *this;
  }

  constexpr variant& operator=(variant&& v) {
    if (this != &v) {
      __destroy__<T0, T...>();
      __move__<T0, T...>(move(v));
    }

    return *this;
  }

  template <contains<T0, T...> U>
  constexpr variant& operator=(const U& u) {
    __destroy__<T0, T...>();
    __copy__(u);
    return *this;
  }

  template <contains<T0, T...> U>
  constexpr variant& operator=(U&& u) {
    __destroy__<T0, T...>();
    __move__(move(u));
    return *this;
  }

 public:
  constexpr size_t index() const { return _index; }

  template <contains<T0, T...> U>
  constexpr U& get() & {
    return *reinterpret_cast<U*>(_data);
  }

  template <contains<T0, T...> U>
  constexpr const U& get() const& {
    return *reinterpret_cast<const U*>(_data);
  }

  template <contains<T0, T...> U>
  constexpr U&& get() && {
    return move(*reinterpret_cast<const U*>(_data));
  }

  template <contains<T0, T...> U>
  constexpr const U&& get() const&& {
    return move(*reinterpret_cast<const U*>(_data));
  }
};

template <typename U, typename... T>
constexpr size_t indexof<U, variant<T...>> = indexof<U, T...>;

template <typename E>
class result_error {
 private:
  E _error;

 public:
  constexpr ~result_error() = default;
  constexpr result_error() = default;
  constexpr result_error(const E& error) : _error(error) {}
  constexpr result_error(const result_error&) = default;
  constexpr result_error(result_error&&) = default;
  constexpr result_error& operator=(const result_error&) = default;
  constexpr result_error& operator=(result_error&&) = default;

 public:
  constexpr const E& error() const { return _error; }
};

template <typename T, typename E>
class result {
 private:
  variant<T, E> _data;

 public:
  constexpr ~result() = default;
  constexpr result(const T& t) : _data(t) {}
  constexpr result(T&& t) : _data(t) {}
  constexpr result(const E& e) : _data(e) {}
  constexpr result(E&& e) : _data(e) {}
  constexpr result(const result&) = default;
  constexpr result(result&&) = default;

  constexpr result& operator=(const result&) = default;
  constexpr result& operator=(result&&) = default;

  constexpr result& operator=(const T& t) {
    _data = t;
    return *this;
  }

  constexpr result& operator=(T&& t) {
    _data = move(t);
    return *this;
  }

  constexpr result& operator=(const E& e) {
    _data = e;
    return *this;
  }

  constexpr result& operator=(E&& e) {
    _data = move(e);
    return *this;
  }

 public:
  constexpr bool has() const { return _data.index() == 0; }
  constexpr T& get() & { return _data.template get<T>(); }
  constexpr const T& get() const& { return _data.template get<T>(); }
  constexpr T&& get() && { return move(_data.template get<T>()); }
  constexpr const T&& get() const&& { return move(_data.template get<T>()); }
  constexpr E& err() & { return _data.template get<E>(); }
  constexpr const E& err() const& { return _data.template get<E>(); }
  constexpr E&& err() && { return move(_data.template get<E>()); }
  constexpr const E&& err() const&& { return move(_data.template get<E>()); }
};

template <typename T>
class maybe {
 private:
  alignas(T) char _data[sizeof(T)];
  bool _has = false;

 public:
  constexpr ~maybe() {
    if (_has) {
      reinterpret_cast<T*>(_data)->~T();
      _has = false;
    }
  }

  constexpr maybe() = default;

  constexpr maybe(const maybe& o) {
    _has = o._has;

    if (_has) {
      new (_data) T(*reinterpret_cast<const T*>(o._data));
    }
  }

  constexpr maybe(maybe&& o) {
    _has = o._has;

    if (_has) {
      new (_data) T(move(*reinterpret_cast<T*>(o._data)));
    }
  }

  constexpr maybe(const T& t) {
    _has = true;
    new (_data) T(t);
  }

  constexpr maybe(T&& t) {
    _has = true;
    new (_data) T(move(t));
  }

  constexpr maybe& operator=(const maybe& o) {
    if (this != &o) {
      if (_has) {
        reinterpret_cast<T*>(_data)->~T();
        _has = false;
      }

      _has = o._has;

      if (_has) {
        new (_data) T(*reinterpret_cast<const T*>(o._data));
      }
    }

    return *this;
  }

  constexpr maybe& operator=(maybe&& o) {
    if (this != &o) {
      if (_has) {
        reinterpret_cast<T*>(_data)->~T();
        _has = false;
      }

      _has = o._has;

      if (_has) {
        new (_data) T(move(*reinterpret_cast<const T*>(o._data)));
      }
    }

    return *this;
  }

  constexpr maybe& operator=(const T& t) {
    if (_has) {
      reinterpret_cast<T*>(_data)->~T();
    }

    new (_data) T(t);
    _has = true;
    return *this;
  }

  constexpr maybe& operator=(T&& t) {
    if (_has) {
      reinterpret_cast<T*>(_data)->~T();
    }

    new (_data) T(move(t));
    _has = true;
    return *this;
  }

  constexpr bool has() const { return _has; }
  constexpr T& get() & { return *reinterpret_cast<T*>(_data); }
  constexpr T&& get() && { return move(*reinterpret_cast<T*>(_data)); }
  constexpr const T& get() const& { return *reinterpret_cast<const T*>(_data); }
  constexpr const T&& get() const&& {
    return move(*reinterpret_cast<const T*>(_data));
  }
};

#endif
