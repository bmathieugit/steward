#ifndef __steward_tuple_hpp__
#define __steward_tuple_hpp__

#include <stew/meta.hpp>
#include <stew/refs.hpp>
#include <stew/utils.hpp>

namespace stew {
template <typename... T>
class tuple;

template <>
class tuple<> {};

template <typename T0, typename... Tn>
class tuple<T0, Tn...> : public tuple<Tn...> {
 private:
  universal_reference<T0> _t;

 public:
  constexpr ~tuple() = default;
  constexpr tuple() = default;
  constexpr tuple(const tuple &) = default;
  constexpr tuple(tuple &&) = default;
  constexpr tuple &operator=(const tuple &) = default;
  constexpr tuple &operator=(tuple &&) = default;

 public:
  template <convertible_to<T0> U0, convertible_to<Tn>... Un>
  constexpr tuple(U0 &&u0, Un &&...un)
      : _t{relay<U0>(u0)}, tuple<Tn...>(relay<Un>(un)...) {}

 public:
  template <size_t I>
  constexpr auto get() & -> decltype(auto) {
    if constexpr (I == 0) {
      return _t.get();
    } else {
      return (tuple<Tn...>::template get<I - 1>());
    }
  }

  template <size_t I>
  constexpr auto get() && -> decltype(auto) {
    if constexpr (I == 0) {
      return _t.get();
    } else {
      return (tuple<Tn...>::template get<I - 1>());
    }
  }

  template <size_t I>
  constexpr auto get() const & -> decltype(auto) {
    if constexpr (I == 0) {
      return _t.get();
    } else {
      return (tuple<Tn...>::template get<I - 1>());
    }
  }

  template <size_t I>
  constexpr auto get() const && -> decltype(auto) {
    if constexpr (I == 0) {
      return _t.get();
    } else {
      return (tuple<Tn...>::template get<I - 1>());
    }
  }
};

template <typename... T>
tuple(T...) -> tuple<T...>;

template <size_t I, typename T0, typename... Tn>
constexpr auto get(tuple<T0, Tn...> &t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... Tn>
constexpr auto get(tuple<T0, Tn...> &&t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... Tn>
constexpr auto get(const tuple<T0, Tn...> &t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... Tn>
constexpr auto get(const tuple<T0, Tn...> &&t) -> decltype(auto) {
  return t.template get<I>();
}

template <typename T0, typename T1>
using pair = tuple<T0, T1>;

}  // namespace stew

#endif
