#ifndef __steward_functional_hpp__
#define __steward_functional_hpp__

#include <stew/smarts.hpp>

namespace stew {
template <typename F>
class function;

template <typename R, typename... A>
class function<R(A...)> {
 private:
  class basic_function_handler {
   public:
    virtual ~basic_function_handler() = default;
    virtual R invoke(A &&...args) = 0;
    virtual basic_function_handler *clone() = 0;
  };

  template <typename F>
  class function_handler : virtual public basic_function_handler {
   private:
    F _func;

   public:
    virtual ~function_handler() = default;
    function_handler() = default;
    function_handler(F &&func) : _func(relay<F>(func)) {}
    function_handler(const function_handler &) = default;
    function_handler(function_handler &&) = default;
    function_handler &operator=(const function_handler &) = default;
    function_handler &operator=(function_handler &&) = default;

   public:
    virtual R invoke(A &&...args) override { return _func(relay<A>(args)...); }

    virtual basic_function_handler *clone() override {
      return new function_handler(*this);
    }
  };

 private:
  owning<basic_function_handler> _handler;
  R(*_func)
  (A...) = nullptr;

 public:
  ~function() = default;
  function() = default;

  function(R (*f)(A...)) : _func(f) {}

  template <typename F>
  function(F &&f) : _handler(new function_handler<F>(relay<F>(f))) {}

  function(const function &o)
      : _handler(o._handler.get() == nullptr ? nullptr : o._handler->clone()),
        _func(o._func) {}

  function(function &&o)
      : _handler(transfer(o._handler)), _func(transfer(o._func)) {}

  function &operator=(function f) {
    _func = transfer(f._func);
    _handler = transfer(f._handler);

    return *this;
  }

 public:
  bool empty() const { return _handler.get() == nullptr && _func == nullptr; }

  template <typename... T>
  R operator()(T &&...t) {
    return _handler.get() != nullptr ? _handler->invoke(relay<T>(t)...)
                                     : _func(relay<T>(t)...);
  }
};

template <typename T>
constexpr auto val(T &&t) {
  return [&t]<typename... A>(A &&...args) -> decltype(auto) {
    return relay<T>(t);
  };
};

template <size_t N>
struct placeholder {
  template <typename T0, typename... Tn>
  constexpr auto operator()(T0 &&t0, Tn &&...tn) const -> decltype(auto) {
    if constexpr (N == 0) {
      return relay<T0>(t0);
    } else {
      return placeholder<N - 1>{}(relay<Tn>(tn)...);
    }
  }

  template <typename O>
  auto operator=(O op) const -> decltype(auto) {
    return [this, op]<typename... A>(A &&...args) {
      return (*this)(relay<A>(args)...) = op(relay<A>(args)...);
    };
  }
};

template <typename OP>
struct operation {
  OP op;

  template <typename... T>
  constexpr auto operator()(T &&...t) const -> decltype(auto) {
    return op(relay<T>(t)...);
  }
};

constexpr auto p0 = placeholder<0>{};
constexpr auto p1 = placeholder<1>{};
constexpr auto p2 = placeholder<2>{};
constexpr auto p3 = placeholder<3>{};
constexpr auto p4 = placeholder<4>{};
constexpr auto p5 = placeholder<5>{};
constexpr auto p6 = placeholder<6>{};
constexpr auto p7 = placeholder<7>{};
constexpr auto p8 = placeholder<8>{};
constexpr auto p9 = placeholder<9>{};

namespace impl {
template <typename T>
constexpr bool placeholder_like = false;

template <size_t N>
constexpr bool placeholder_like<placeholder<N>> = true;

template <typename T>
constexpr bool placeholder_like<operation<T>> = true;

}  // namespace impl

template <typename T>
concept placeholder_like = impl::placeholder_like<T>;

template <size_t N0>
constexpr auto operator++(placeholder<N0> pn0) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return ++pn0(relay<A>(args)...); }};
}

template <size_t N0>
constexpr auto operator++(placeholder<N0> pn0, int) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return pn0(relay<A>(args)...)++; }};
}

template <size_t N0>
constexpr auto operator--(placeholder<N0> pn0) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return --pn0(relay<A>(args)...); }};
}

template <size_t N0>
constexpr auto operator--(placeholder<N0> pn0, int) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return pn0(relay<A>(args)...)--; }};
}

template <size_t N0>
constexpr auto operator!(placeholder<N0> pn0) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return !pn0(relay<A>(args)...); }};
}

template <size_t N0>
constexpr auto operator+(placeholder<N0> pn0) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return +pn0(relay<A>(args)...); }};
}

template <size_t N0>
constexpr auto operator-(placeholder<N0> pn0) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return -pn0(relay<A>(args)...); }};
}

template <size_t N0>
constexpr auto operator~(placeholder<N0> pn0) -> decltype(auto) {
  return operation{
      [pn0]<typename... A>(A &&...args) { return ~pn0(relay<A>(args)...); }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator+(P0 p0, P1 p1) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) + p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator-(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) - p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator/(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) / p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator*(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) * p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator%(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) % p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator|(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) | p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator&(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) & p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator^(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) ^ p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator||(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) || p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator&&(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) && p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator<<(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) << p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator>>(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) >> p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator==(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) == p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator!=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) != p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator<(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) < p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator>(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) > p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator<=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) <= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator>=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) >= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator<=>(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) <=> p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator+=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) += p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator-=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) -= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator/=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) /= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator*=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) *= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator%=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) %= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator&=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) &= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator|=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) |= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator^=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) ^= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator>>=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) >>= p1(relay<A>(args)...);
  }};
}

template <typename P0, typename P1>
  requires(placeholder_like<P0> || placeholder_like<P1>)
constexpr auto operator<<=(P0 p0, P1 p1) -> decltype(auto) {
  return operation{[p0, p1]<typename... A>(A &&...args) {
    return p0(relay<A>(args)...) <<= p1(relay<A>(args)...);
  }};
}

}  // namespace stew

#endif
