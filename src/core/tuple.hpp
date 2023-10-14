#ifndef __n_tuple_hpp__
#define __n_tuple_hpp__


template <typename... T>
class tuple;

template <>
class tuple<> {};

template <typename T>
struct tuple_item {
  T _t;

  constexpr T& get() & { return _t; }
  constexpr const T& get() const& { return _t; }
  constexpr T&& get() && { return _t; }
  constexpr const T&& get() const&& { return _t; }
};

// template <typename T>
// struct tuple_item<T&&> {
//   T&& _t;

//   constexpr T&& get() & { return _t; }
//   constexpr const T&& get() const& { return _t; }
//   constexpr T&& get() && { return _t; }
//   constexpr const T&& get() const&& { return _t; }
// };

// template <typename T>
// struct tuple_item<const T&&> {
//   const T&& _t;

//   constexpr const T&& get() & { return _t; }
//   constexpr const T&& get() const& { return _t; }
//   constexpr const T&& get() && { return _t; }
//   constexpr const T&& get() const&& { return _t; }
// };

// template <typename T>
// struct tuple_item<const T&> {
//   const T& _t;

//   constexpr const T& get() & { return _t; }
//   constexpr const T& get() const& { return _t; }
//   constexpr const T&& get() && { return _t; }
//   constexpr const T&& get() const&& { return _t; }
// };

template <typename T0, typename... TN>
class tuple<T0, TN...>: public tuple<TN...> {
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
  /*  template <convertible_to<T0> U0, convertible_to<TN>... Un>
    constexpr tuple(U0&& u0, Un&&... un)
        : _t{relay<U0>(u0)}, tuple<TN...>(relay<Un>(un)...) {}
  */

  constexpr tuple(const T0& t0, const TN&... tn) : tuple<TN...>(tn...), _item{t0} {}

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
};

template <typename... T>
tuple(T...) -> tuple<T...>;


template <size_t I, typename T0, typename... TN>
constexpr auto get(tuple<T0, TN...> &t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... TN>
constexpr auto get(tuple<T0, TN...> &&t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... TN>
constexpr auto get(const tuple<T0, TN...> &t) -> decltype(auto) {
  return t.template get<I>();
}

template <size_t I, typename T0, typename... TN>
constexpr auto get(const tuple<T0, TN...> &&t) -> decltype(auto) {
  return t.template get<I>();
}



#endif