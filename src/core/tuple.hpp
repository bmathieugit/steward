#ifndef __n_tuple_hpp__
#define __n_tuple_hpp__

#include <core/core.hpp>

template <size_t... I>
struct index_sequence {};

template <size_t N, size_t... I>
struct __make_index_sequence : __make_index_sequence<N - 1, N - 1, I...> {};

template <size_t... I>
struct __make_index_sequence<0, I...> {
  using type = index_sequence<I...>;
};

template <size_t N>
using make_index_sequence = typename __make_index_sequence<N>::type;

template <typename T>
struct tuple_item {
  T _t;

  constexpr T& get() & { return _t; }
  constexpr const T& get() const& { return _t; }
  constexpr T&& get() && { return _t; }
  constexpr const T&& get() const&& { return _t; }
};

template <typename T>
struct tuple_item<T&&> {
  T&& _t;

  constexpr T&& get() & { return _t; }
  constexpr const T&& get() const& { return _t; }
  constexpr T&& get() && { return _t; }
  constexpr const T&& get() const&& { return _t; }
};

template <typename T>
struct tuple_item<const T&&> {
  const T&& _t;

  constexpr const T&& get() & { return _t; }
  constexpr const T&& get() const& { return _t; }
  constexpr const T&& get() && { return _t; }
  constexpr const T&& get() const&& { return _t; }
};

template <typename T>
struct tuple_item<const T&> {
  const T& _t;

  constexpr const T& get() & { return _t; }
  constexpr const T& get() const& { return _t; }
  constexpr const T&& get() && { return _t; }
  constexpr const T&& get() const&& { return _t; }
};

template <typename... T>
class tuple;

template <>
class tuple<> {
 public:
  template <typename T>
  constexpr tuple<T> append(T&& t) const {
    return tuple<T>(relay<T>(t));
  }
};

template <typename T0, typename... TN>
class tuple<T0, TN...> : public tuple<TN...> {
 private:
  tuple_item<T0> _item;

 public:
  constexpr ~tuple() = default;
  constexpr tuple() = default;
  constexpr tuple(const tuple&) = default;
  constexpr tuple(tuple&&) = default;
  constexpr tuple& operator=(const tuple&) = default;
  constexpr tuple& operator=(tuple&&) = default;

 public:
  template <convertible_to<T0> U0, convertible_to<TN>... Un>
  constexpr tuple(U0&& u0, Un&&... un)
      : tuple<TN...>(relay<Un>(un)...), _item{relay<U0>(u0)} {}

 public:
  template <size_t I>
  constexpr auto get() & -> decltype(auto) {
    if constexpr (I == 0) {
      return _item.get();
    } else {
      return (tuple<TN...>::template get<I - 1>());
    }
  }

  template <size_t I>
  constexpr auto get() && -> decltype(auto) {
    if constexpr (I == 0) {
      return _item.get();
    } else {
      return (tuple<TN...>::template get<I - 1>());
    }
  }

  template <size_t I>
  constexpr auto get() const& -> decltype(auto) {
    if constexpr (I == 0) {
      return _item.get();
    } else {
      return (tuple<TN...>::template get<I - 1>());
    }
  }

  template <size_t I>
  constexpr auto get() const&& -> decltype(auto) {
    if constexpr (I == 0) {
      return _item.get();
    } else {
      return (tuple<TN...>::template get<I - 1>());
    }
  }

 private:
  template <typename T, size_t... I>
  constexpr tuple<T0, TN..., T> append(T&& t, index_sequence<I...>) const {
    return tuple<T0, TN..., T>(this->template get<I>()..., relay<T>(t));
  }

 public:
  template <typename T>
  constexpr tuple<T0, TN..., T> append(T&& t) const {
    return append(relay<T>(t), make_index_sequence<sizeof...(TN)>{});
  }
};

template <typename... T>
tuple(T...) -> tuple<T...>;

template <size_t I, typename T0, typename... TN>
constexpr auto get(tuple<T0, TN...>& t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... TN>
constexpr auto get(tuple<T0, TN...>&& t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... TN>
constexpr auto get(const tuple<T0, TN...>& t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... TN>
constexpr auto get(const tuple<T0, TN...>&& t) -> decltype(auto) {
  return t.template get<I>();
}

template <typename T0, typename T1>
using pair = tuple<T0, T1>;

template <typename... T0, typename... T1, size_t... I0, size_t... I1>
constexpr tuple<T0..., T1...> __concat(const tuple<T0...>& t0,
                                       const tuple<T1...>& t1,
                                       index_sequence<I0...>,
                                       index_sequence<I1...>) {
  return tuple<T0..., T1...>(get<I0>(t0)..., get<I1>(t1)...);
}

template <typename... T0, typename... T1>
constexpr tuple<T0..., T1...> concat(const tuple<T0...>& t0,
                                     const tuple<T1...>& t1) {
  return __concat(t0, t1, make_index_sequence<sizeof...(T0)>{},
                  make_index_sequence<sizeof...(T1)>{});
}

#endif