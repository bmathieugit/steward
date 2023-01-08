#ifndef __stew_hpp__
#define __stew_hpp__

#include <clibs.hpp>

namespace stew
{
  using nullptr_t = decltype(nullptr);
  using size_t = unsigned long;

  //--------------------------------
  //
  // Metaprogramming
  //
  //--------------------------------

  template <typename T>
  struct struct_type
  {
    using type = T;
  };

  template <typename T>
  using type = typename T::type;

  template <typename T0, typename... Tn>
  consteval size_t sizeofmax()
  {
    if constexpr (sizeof...(Tn) == 0)
    {
      return sizeof(T0);
    }
    else
    {
      if (sizeof(T0) > sizeofmax<Tn...>())
      {
        return sizeof(T0);
      }
      else
      {
        return sizeofmax<Tn...>();
      }
    }
  }

  namespace impl
  {
    template <size_t I, typename H, typename... T>
    struct indexof
    {
      static_assert(sizeof...(T) > 0, "indexof : search type not in types pack");
    };

    template <size_t I, typename H, typename T0, typename... Tn>
    struct indexof<I, H, T0, Tn...>
    {
      static constexpr size_t value = indexof<I + 1, H, Tn...>::value;
    };

    template <size_t I, typename T, typename... Tn>
    struct indexof<I, T, T, Tn...>
    {
      static constexpr size_t value = I;
    };
  }

  template <typename H, typename... T>
  constexpr size_t indexof = impl::indexof<0, H, T...>::value;

  namespace impl
  {
    template <size_t I, typename T0, typename... Tn>
    struct typeat : struct_type<typeat<I - 1, Tn...>>
    {
    };

    template <typename T0, typename... Tn>
    struct typeat<0, T0, Tn...> : struct_type<T0>
    {
    };
  }

  template <size_t I, typename... T>
  using typeat = type<impl::typeat<I, T...>>;

  namespace impl
  {
    template <typename T>
    struct rm_ref : struct_type<T>
    {
    };

    template <typename T>
    struct rm_ref<T &> : struct_type<T>
    {
    };

    template <typename T>
    struct rm_ref<T &&> : struct_type<T>
    {
    };

    template <typename T>
    struct rm_const : struct_type<T>
    {
    };

    template <typename T>
    struct rm_const<const T> : struct_type<T>
    {
    };

    template <typename T>
    struct rm_volatile : struct_type<T>
    {
    };

    template <typename T>
    struct rm_volatile<volatile T> : struct_type<T>
    {
    };

    template <typename T>
    struct rm_array : struct_type<T>
    {
    };

    template <typename T>
    struct rm_array<T[]> : struct_type<T>
    {
    };

    template <typename T>
    struct rm_pointer : struct_type<T>
    {
    };

    template <typename T>
    struct rm_pointer<T *> : struct_type<T>
    {
    };
  }

  template <typename T>
  using add_const = const T;

  template <typename T>
  using rm_const = type<impl::rm_const<T>>;

  template <typename T>
  using add_volatile = volatile T;

  template <typename T>
  using rm_volatile = type<impl::rm_volatile<T>>;

  template <typename T>
  using add_lref = T &;

  template <typename T>
  using add_rref = T &&;

  template <typename T>
  using rm_ref = type<impl::rm_ref<T>>;

  template <typename T>
  using rm_cvref = rm_const<rm_volatile<rm_ref<T>>>;

  template <typename T>
  using add_array = T[];

  template <typename T>
  using rm_array = type<impl::rm_array<T>>;

  template <typename T>
  using add_pointer = T *;

  template <typename T>
  using rm_pointer = type<impl::rm_pointer<T>>;

  //-----------------------------------
  //
  // Generals concepts
  //
  //-----------------------------------

  namespace impl
  {
    template <typename T>
    constexpr bool lvalue_reference_like = false;

    template <typename T>
    constexpr bool lvalue_reference_like<T &> = true;
  }

  template <typename T>
  concept lvalue_reference_like = impl::lvalue_reference_like<T>;

  template <typename T>
  concept not_lvalue_reference_like = (!lvalue_reference_like<T>);

  namespace impl
  {
    template <typename T>
    constexpr bool const_lvalue_reference_like = false;

    template <typename T>
    constexpr bool const_lvalue_reference_like<const T &> = true;
  }

  template <typename T>
  concept const_lvalue_reference_like = impl::const_lvalue_reference_like<T>;

  namespace impl
  {
    template <typename T>
    constexpr bool rvalue_reference_like = false;

    template <typename T>
    constexpr bool rvalue_reference_like<T &&> = true;
  }

  template <typename T>
  concept rvalue_reference_like = impl::rvalue_reference_like<T>;

  namespace impl
  {
    template <typename T>
    constexpr bool pointer_like = false;

    template <typename T>
    constexpr bool pointer_like<T *> = true;
  }

  template <typename T>
  concept pointer_like = impl::pointer_like<T>;

  namespace impl
  {
    template <typename T, typename U>
    constexpr bool same_as = false;

    template <typename T>
    constexpr bool same_as<T, T> = true;
  }

  template <typename T, typename U>
  concept same_as = impl::same_as<T, U>;

  template <typename T, typename... U>
  concept same_one_of = (same_as<T, U> || ...);

  template <typename T, size_t N>
  concept size_more_than = (sizeof(T) > N);

  template <typename T, size_t N>
  concept size_lesseq_than = (sizeof(T) <= N);

  template <typename From, typename To>
  concept convertible_to =
      requires { static_cast<rm_cvref<To>>(*((rm_cvref<From> *)nullptr)); };

  template <typename T, typename R, typename... A>
  concept callable = requires(T t, A &&...a) { 
    {t(a...)} -> convertible_to<R>; };

  template <typename P, typename... A>
  concept predicate = callable<P, bool, A...>;

  template <typename T, typename O>
  concept equal_comparable =
      requires(const T &t, const O &o) {
        {
          t == o
          } -> convertible_to<bool>;
      };

  template <typename T>
  concept character = same_one_of<T, char, wchar_t>;

  template <typename T>
  concept signed_integral =
      same_one_of<T, short, int, long, long long>;

  template <typename T>
  concept unsigned_integral =
      same_one_of<T, unsigned short,
                  unsigned int,
                  unsigned long,
                  unsigned long long>;

  template <typename T>
  concept integral =
      signed_integral<T> || unsigned_integral<T>;

  template <typename T>
  concept floating_point = same_one_of<T, float, double, long double>;

  template <typename T>
  concept native_number = integral<T> ||
                          character<T> ||
                          floating_point<T> ||
                          same_as<T, bool>;

  namespace impl
  {
    template <typename T>
    constexpr bool pointer = false;

    template <typename T>
    constexpr bool pointer<T *> = true;
  }

  template <typename T>
  concept pointer = impl::pointer<T>;

  namespace impl
  {
    template <typename T>
    constexpr bool native_array_like = false;

    template <typename T>
    constexpr bool native_array_like<T[]> = true;

    template <typename T, size_t N>
    constexpr bool native_array_like<T[N]> = true;

    template <typename T, size_t N>
    constexpr bool native_array_like<T (&)[N]> = true;
  }

  template <typename T>
  concept native_array_like = impl::native_array_like<T>;

  template <typename D, typename B>
  constexpr bool derived_from =
      requires { dynamic_cast<B *>(static_cast<D *>(nullptr)); } &&
      requires { dynamic_cast<B &>(*static_cast<D *>(nullptr)); };

  //-----------------------------------
  //
  // Utilities functions
  //
  //-----------------------------------

  template <typename T>
  constexpr T &&forward(rm_ref<T> &t) noexcept
  {
    return static_cast<T &&>(t);
  }

  template <not_lvalue_reference_like T>
  constexpr T &&forward(rm_ref<T> &&t) noexcept
  {
    return static_cast<T &&>(t);
  }

  template <typename T>
  constexpr rm_ref<T> &&transfer(T &&t)
  {
    return static_cast<rm_ref<T> &&>(t);
  }

  template <native_number T>
  constexpr auto transfer(T &t)
  {
    T copy = static_cast<rm_ref<T> &&>(t);
    t = 0;
    return copy;
  }

  template <pointer T>
  constexpr auto transfer(T &p)
  {
    auto copy = p;
    p = nullptr;
    return copy;
  }

  //---------------------------------
  //
  // Smart pointers
  //
  //---------------------------------

  template <typename T>
  class non_owning
  {
  private:
    rm_array<T> *_ptr = nullptr;

  public:
    ~non_owning() = default;
    non_owning() = default;

    template <typename U>
    non_owning(U *ptr) : _ptr(ptr) {}

    non_owning(nullptr_t) : non_owning() {}

    non_owning(const non_owning &) = default;
    non_owning(non_owning &&) = default;

    template <typename U>
    non_owning &operator=(U *ptr)
    {
      if (_ptr != ptr)
      {
        _ptr = ptr;
      }

      return *this;
    }

    non_owning &operator=(nullptr_t)
    {
      _ptr = nullptr;
      return *this;
    }

    non_owning &operator=(const non_owning &) = default;
    non_owning &operator=(non_owning &&) = default;

  public:
    auto get() const
    {
      return _ptr;
    }

  public:
    operator bool() const
    {
      return _ptr != nullptr;
    }

    auto operator*() const
        -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return (*_ptr);
    }

    auto operator->() const
        -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return _ptr;
    }

    auto operator[](size_t i) const
        -> decltype(auto)
      requires(native_array_like<T>)
    {
      return _ptr[i];
    }
  };

  template <typename T>
  class owning
  {
  private:
    rm_array<T> *_ptr = nullptr;

  private:
    void cleanup()
    {
      if (_ptr != nullptr)
      {
        if constexpr (native_array_like<T>)
        {
          delete[] _ptr;
        }
        else
        {
          delete _ptr;
        }

        _ptr = nullptr;
      }
    }

  public:
    ~owning()
    {
      cleanup();
    }

    owning() = default;

    owning(rm_array<T> *ptr)
        : _ptr(ptr) {}

    owning(const owning &) = delete;
    owning(owning &&o)
        : _ptr(transfer(o._ptr))
    {
    }

    owning &operator=(const owning &) = delete;
    owning &operator=(owning &&o)
    {
      assert(this != &o);
      cleanup();
      _ptr = transfer(o._ptr);
      return *this;
    }

    owning &operator=(rm_array<T> *ptr)
    {
      assert(_ptr != ptr);
      cleanup();
      _ptr = ptr;
      return *this;
    }

  public:
    auto get() const
    {
      return _ptr;
    }

  public:
    auto operator*() const
        -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return (*_ptr);
    }

    auto operator->() const
        -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return _ptr;
    }

    auto operator[](size_t i) const
        -> decltype(auto)
      requires native_array_like<T>
    {
      return _ptr[i];
    }
  };

  template <typename T>
  class sharing;

  //---------------------------------
  //
  // References container
  //
  //---------------------------------

  template <typename T>
  class reference
  {
  private:
    non_owning<T> _t;

  public:
    constexpr ~reference() = default;
    constexpr reference(T &t) : _t(&t) {}
    constexpr reference(const reference &) = default;
    constexpr reference &operator=(const reference &) = default;

  public:
    constexpr operator T &() const
    {
      return *_t;
    }

    constexpr T &get() const
    {
      return *_t;
    }
  };

  template <typename T>
  class const_reference
  {
  private:
    non_owning<const T> _t;

  public:
    constexpr ~const_reference() = default;
    constexpr const_reference(const T &t) : _t(&t) {}
    constexpr const_reference(const const_reference &) = default;
    constexpr const_reference &operator=(const const_reference &) = default;

  public:
    constexpr operator const T &() const
    {
      return *_t;
    }

    constexpr const T &get() const
    {
      return *_t;
    }
  };

  template <typename T>
  class move_reference
  {
  private:
    non_owning<T> _t;

  public:
    constexpr ~move_reference() = default;
    constexpr move_reference(T &&t) : _t(&t) {}
    constexpr move_reference(const move_reference &) = default;
    constexpr move_reference &operator=(const move_reference &) = default;

  public:
    constexpr operator T &&() const
    {
      return *_t;
    }

    constexpr T &&get() const
    {
      return *_t;
    }
  };

  template <typename T>
  class forward_reference
  {
  private:
    T _t;

  public:
    constexpr ~forward_reference() = default;
    constexpr forward_reference() = default;
    constexpr forward_reference(T &&t) : _t(t) {}
    constexpr forward_reference(const forward_reference &) = default;
    constexpr forward_reference &operator=(const forward_reference &) = default;

  public:
    constexpr operator T &()
    {
      return _t;
    }

    constexpr T &get()
    {
      return _t;
    }
  };

  template <typename T>
  class forward_reference<T &&>
  {
  private:
    move_reference<T> _t;

  public:
    constexpr ~forward_reference() = default;
    constexpr forward_reference(T &&t) : _t(t) {}
    constexpr forward_reference(const forward_reference &) = default;
    constexpr forward_reference &operator=(const forward_reference &) = default;

  public:
    constexpr operator T &&() const
    {
      return _t.get();
    }

    constexpr T &&get() const
    {
      return _t.get();
    }
  };

  template <typename T>
  class forward_reference<T &>
  {
  private:
    reference<T> _t;

  public:
    constexpr ~forward_reference() = default;
    constexpr forward_reference(T &t) : _t(t) {}
    constexpr forward_reference(const forward_reference &) = default;
    constexpr forward_reference &operator=(const forward_reference &) = default;

  public:
    constexpr operator T &() const
    {
      return _t.get();
    }

    constexpr T &get() const
    {
      return _t.get();
    }
  };

  template <typename T>
  class forward_reference<const T &>
  {
  private:
    const_reference<T> _t;

  public:
    constexpr ~forward_reference() = default;
    constexpr forward_reference(const T &t) : _t(t) {}
    constexpr forward_reference(const forward_reference &) = default;
    constexpr forward_reference &operator=(const forward_reference &) = default;

  public:
    constexpr operator const T &() const
    {
      return _t.get();
    }

    constexpr const T &get() const
    {
      return _t.get();
    }
  };

  //----------------------------------
  //
  // Function container
  //
  //----------------------------------

  template <typename F>
  class function;

  template <typename R, typename... A>
  class function<R(A...)>
  {
  private:
    class basic_function_handler
    {
    public:
      virtual ~basic_function_handler() = default;
      virtual R invoke(A &&...args) = 0;
      virtual basic_function_handler *clone() = 0;
    };

    template <typename F>
    class function_handler
        : virtual public basic_function_handler
    {
    private:
      F _func;

    public:
      virtual ~function_handler() = default;
      function_handler() = default;
      function_handler(F &&func) : _func(forward<F>(func)) {}
      function_handler(const function_handler &) = default;
      function_handler(function_handler &&) = default;
      function_handler &operator=(const function_handler &) = default;
      function_handler &operator=(function_handler &&) = default;

    public:
      virtual R invoke(A &&...args) override
      {
        return _func(forward<A>(args)...);
      }

      virtual basic_function_handler *clone() override
      {
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
    function(F &&f) : _handler(new function_handler<F>(forward<F>(f)))
    {
    }

    function(const function &o)
        : _handler(o._handler == nullptr ? nullptr : o._handler->clone()),
          _func(o._func)
    {
    }

    function(function &&o)
        : _handler(transfer(o._handler)),
          _func(transfer(o._func))
    {
    }

    function &operator=(function f)
    {
      _func = transfer(f._func);
      _handler = transfer(f._handler);

      return *this;
    }

  public:
    operator bool() const
    {
      return static_cast<bool>(_handler) || _func != nullptr;
    }

    template <typename... T>
    R operator()(T &&...t)
    {
      return _handler
                 ? _handler->invoke(forward<T>(t)...)
                 : _func(forward<T>(t)...);
    }
  };

  //----------------------------------
  //
  // Tuple container
  //
  //----------------------------------

  template <typename... T>
  class tuple;

  template <>
  class tuple<>
  {
  };

  template <typename T0, typename... Tn>
  class tuple<T0, Tn...> : public tuple<Tn...>
  {
  private:
    forward_reference<T0> _t;

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
        : _t{forward<U0>(u0)}, tuple<Tn...>(forward<Un>(un)...)
    {
    }

  public:
    template <size_t I>
    constexpr auto get() & -> decltype(auto)
    {
      if constexpr (I == 0)
      {
        return _t.get();
      }
      else
      {
        return (tuple<Tn...>::template get<I - 1>());
      }
    }

    template <size_t I>
    constexpr auto get() && -> decltype(auto)
    {
      if constexpr (I == 0)
      {
        return _t.get();
      }
      else
      {
        return (tuple<Tn...>::template get<I - 1>());
      }
    }

    template <size_t I>
    constexpr auto get() const & -> decltype(auto)
    {
      if constexpr (I == 0)
      {
        return _t.get();
      }
      else
      {
        return (tuple<Tn...>::template get<I - 1>());
      }
    }

    template <size_t I>
    constexpr auto get() const && -> decltype(auto)
    {
      if constexpr (I == 0)
      {
        return _t.get();
      }
      else
      {
        return (tuple<Tn...>::template get<I - 1>());
      }
    }
  };

  template <size_t I, typename T0, typename... Tn>
  constexpr auto get(tuple<T0, Tn...> &t) -> decltype(auto)
  {
    return t.template get<I>();
  }

  template <size_t I, typename T0, typename... Tn>
  constexpr auto get(tuple<T0, Tn...> &&t) -> decltype(auto)
  {
    return t.template get<I>();
  }

  template <size_t I, typename T0, typename... Tn>
  constexpr auto get(const tuple<T0, Tn...> &t) -> decltype(auto)
  {
    return t.template get<I>();
  }

  template <size_t I, typename T0, typename... Tn>
  constexpr auto get(const tuple<T0, Tn...> &&t) -> decltype(auto)
  {
    return t.template get<I>();
  }

  namespace todo
  {
    template <class T>
    struct type_identity
    {
      using type = T;
    };

    template <typename T>
    using type_identity_t = typename type_identity<T>::type;

    template <size_t... I>
    struct isequence
    {
    };

    namespace impl
    {
      template <size_t I0, size_t... In>
      consteval auto make_isequence()
      {
        if constexpr (I0 == 0)
        {
          return isequence<I0, In...>{};
        }
        else
        {
          return make_isequence<I0 - 1, I0, In...>();
        }
      }
    }

    template <size_t N>
      requires(N > 0)
    constexpr auto make_isequence()
    {
      return impl::make_isequence<N - 1>();
    }

    template <typename T>
    constexpr size_t array_size = 0;

    template <typename... T>
    constexpr size_t array_size<tuple<T...>> = sizeof...(T);

    template <typename T, typename F, size_t... I>
    consteval auto apply(T &&t, F &&f, isequence<I...>) -> decltype(auto)
    {
      return forward<F>(f)(get<I>(forward<T>(t))...);
    }

    template <typename T, typename F>
    consteval auto apply(T &&t, F &&f) -> decltype(auto)
    {
      return apply(forward<T>(t), forward<F>(f), make_isequence<array_size<rm_cvref<T>>>());
    }
  }

  //----------------------------------
  //
  // Functionnal utilities
  //
  //----------------------------------

  template <typename T>
  constexpr auto val(T &&t)
  {
    return [&t]<typename... A>(A &&...args) -> decltype(auto)
    {
      return forward<T>(t);
    };
  };

  template <size_t N>
  struct placeholder
  {
    template <typename T0, typename... Tn>
    constexpr auto operator()(T0 &&t0, Tn &&...tn) const -> decltype(auto)
    {
      if constexpr (N == 0)
      {
        return forward<T0>(t0);
      }
      else
      {
        return placeholder<N - 1>{}(forward<Tn>(tn)...);
      }
    }

    template <typename O>
    auto operator=(O op) const -> decltype(auto)
    {
      return [this, op]<typename... A>(A &&...args)
      {
        return (*this)(forward<A>(args)...) = op(forward<A>(args)...);
      };
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

  namespace impl
  {
    template <typename T>
    constexpr bool placeholder_like = false;

    template <size_t N>
    constexpr bool placeholder_like<placeholder<N>> = true;
  }

  template <typename T>
  concept placeholder_like = impl::placeholder_like<T>;

  template <size_t N0>
  constexpr auto operator++(placeholder<N0> pn0) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return ++pn0(forward<A>(args)...);
    };
  }

  template <size_t N0>
  constexpr auto operator++(placeholder<N0> pn0, int) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return pn0(forward<A>(args)...)++;
    };
  }

  template <size_t N0>
  constexpr auto operator--(placeholder<N0> pn0) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return --pn0(forward<A>(args)...);
    };
  }

  template <size_t N0>
  constexpr auto operator--(placeholder<N0> pn0, int) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return pn0(forward<A>(args)...)--;
    };
  }

  template <size_t N0>
  constexpr auto operator!(placeholder<N0> pn0) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return !pn0(forward<A>(args)...);
    };
  }

  template <size_t N0>
  constexpr auto operator+(placeholder<N0> pn0) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return +pn0(forward<A>(args)...);
    };
  }

  template <size_t N0>
  constexpr auto operator-(placeholder<N0> pn0) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return -pn0(forward<A>(args)...);
    };
  }

  template <size_t N0>
  constexpr auto operator~(placeholder<N0> pn0) -> decltype(auto)
  {
    return [pn0]<typename... A>(A &&...args)
    {
      return ~pn0(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator+(P0 p0, P1 p1)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) + p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator-(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) - p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator/(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) / p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator*(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) * p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator%(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) % p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator|(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) | p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator&(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) & p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator^(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) ^ p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator||(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) || p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator&&(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) && p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator<<(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) << p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator>>(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) >> p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator==(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) == p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator!=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) != p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator<(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) < p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator>(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) > p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator<=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) <= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator>=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) >= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator<=>(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) <=> p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator+=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) += p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator-=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) -= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator/=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) /= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator*=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) *= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator%=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) %= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator&=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) &= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator|=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) |= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator^=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) ^= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator>>=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) >>= p1(forward<A>(args)...);
    };
  }

  template <typename P0, typename P1>
    requires(placeholder_like<P0> || placeholder_like<P1>)
  constexpr auto operator<<=(P0 p0, P1 p1) -> decltype(auto)
  {
    return [p0, p1]<typename... A>(A &&...args)
    {
      return p0(forward<A>(args)...) <<= p1(forward<A>(args)...);
    };
  }

  //---------------------------
  //
  // Algorithm facilities
  //
  //---------------------------

  template <typename T>
  concept forward_iterator =
      requires(T i) {
        ++i;
        i++;
        i != i;
        i == i;
        *i;
        i - i;
      };

  template <typename T>
  concept backward_iterator =
      requires(T i) {
        --i;
        i--;
        i != i;
        i == i;
        *i;
        i - i;
      };

  template <typename C>
  concept strict_range =
      requires(C &c) {
        { c.begin() } -> forward_iterator;
        { c.end() } -> forward_iterator; };

  template <typename C>
  concept range =
      native_array_like<C> ||
      strict_range<C>;

  template <typename C>
  concept push_back_range =
      range<C> &&
      requires(C &c,
               const decltype(*c.begin()) &i1,
               decltype(*c.begin()) &&i2) {
        c.push_back(i1);
        c.push_back(i2);
      };

  template <typename C>
  concept push_range =
      range<C> &&
      requires(C &c,
               const decltype(*c.begin()) &i1,
               decltype(*c.begin()) &&i2) {
        c.push(i1);
        c.push(i2);
      };

  template <range R>
  struct range_traits
  {
    using value_type = rm_cvref<decltype(*(R{}.begin()))>;
    using reference = value_type &;
    using pointer = value_type *;
    using const_reference = const value_type &;
  };

  template <range R>
  using range_value_type = typename range_traits<R>::value_type;

  template <range R>
  using range_reference = typename range_traits<R>::reference;

  template <range R>
  using range_pointer = typename range_traits<R>::pointer;

  template <range R>
  using range_const_reference = typename range_traits<R>::const_reference;

  template <typename T>
  concept random_accessible =
      requires(T t) { t[0]; };

  template <strict_range R>
  constexpr auto begin(R &&r)
  {
    return forward<R>(r).begin();
  }

  template <strict_range R>
  constexpr auto end(R &&r)
  {
    return forward<R>(r).end();
  }

  template <typename T, size_t N>
  constexpr auto begin(T (&t)[N])
  {
    return t;
  }

  template <typename T, size_t N>
  constexpr auto end(T (&t)[N])
  {
    return t + N;
  }

  template <forward_iterator I>
  class view
  {
  private:
    I _begin;
    I _end;

  public:
    constexpr ~view() = default;
    constexpr view() = default;
    constexpr view(I b, I e)
        : _begin(b), _end(e) {}

    template <range R>
    constexpr view(const R &r)
        : view(stew::begin(r), stew::end(r))
    {
    }

    constexpr view(const view &) = default;
    constexpr view(view &&) = default;

    constexpr view &operator=(const view &) = default;
    constexpr view &operator=(view &&) = default;

  public:
    constexpr auto begin()
    {
      return _begin;
    }

    constexpr auto end()
    {
      return _end;
    }

    constexpr auto begin() const
    {
      return _begin;
    }

    constexpr auto end() const
    {
      return _end;
    }

  public:
    constexpr auto size() const
    {
      return _begin == nullptr ? 0 : _end - _begin;
    }

    constexpr auto empty() const
    {
      return size() == 0;
    }

    constexpr const auto &operator[](size_t i) const
      requires random_accessible<I>
    {
      return (_begin[i]);
    }
  };

  template <range R>
  view(R &&r) -> view<decltype(begin(forward<R>(r)))>;

  template <range R1, range R2>
  constexpr bool equals(const R1 &r1, const R2 &r2)
  {
    if (&r1 == &r2)
    {
      return true;
    }
    else
    {
      auto b1 = begin(r1);
      auto b2 = begin(r2);

      auto e1 = end(r1);
      auto e2 = end(r2);

      while (b1 != e1 &&
             b2 != e2 &&
             *b1 == *b2)
      {
        ++b1;
        ++b2;
      }

      return b1 == e1 && b2 == e2;
    }
  }

  template <range R1, range R2>
  bool operator==(const R1 &r1, const R2 &r2)
  {
    return stew::equals(r1, r2);
  }

  template <range R1, range R2>
  bool operator!=(const R1 &r1, const R2 &r2)
  {
    return !stew::equals(r1, r2);
  }

  template <range R1, range R2>
  constexpr bool starts_with(const R1 &r1, const R2 &r2)
  {
    auto b1 = begin(r1);
    auto b2 = begin(r2);

    auto e1 = end(r1);
    auto e2 = end(r2);

    while (b1 != e1 &&
           b2 != e2 &&
           *b1 == *b2)
    {
      ++b1;
      ++b2;
    }

    return b2 == e2;
  }

  template <range R, typename T>
  constexpr bool starts_with(const R &r, const T &t)
  {
    auto b = begin(r);
    return b != end(r) && *b == t;
  }

  template <range R, typename T>
  constexpr auto find(R &r, const T &t)
  {
    auto b = begin(r);
    auto e = end(r);

    while (b != e && *b != t)
    {
      ++b;
    }

    return b;
  }

  template <range R1, range R2>
  constexpr auto find(R1 &r1, const R2 &r2)
  {
    auto b1 = begin(r1);
    auto e1 = end(r1);

    while (b1 != e1 && !starts_with(view(b1, e1), r2))
    {
      ++b1;
    }

    return b1;
  }

  template <range R, predicate<range_const_reference<R>> P>
  constexpr auto find(R &r, P &&pred)
  {
    auto b = begin(r);
    auto e = end(r);

    while (b != e && !forward<P>(pred)(*b))
    {
      ++b;
    }

    return b;
  }

  template <range R, equal_comparable<range_value_type<R>> T>
  constexpr size_t count(const R &r, T &&t)
  {
    size_t c = 0;

    for (const auto &i : r)
    {
      if (i == forward<T>(t))
      {
        ++c;
      }
    }

    return c;
  }

  template <range R, predicate<range_const_reference<R>> P>
  constexpr size_t count(const R &r, P &&pred)
  {
    size_t c = 0;

    for (const auto &i : r)
    {
      if (forward<P>(pred)(i))
      {
        ++c;
      }
    }

    return c;
  }

  template <range R, typename T>
  constexpr bool contains(R &r, const T &t)
  {
    return find(r, t) != end(r);
  }

  template <range R1, range R2>
  constexpr bool contains(const R1 &r1, const R2 &r2)
  {
    return find(r1, r2) != end(r1);
  }

  template <forward_iterator I>
  struct around_pair
  {
    bool _found;
    view<I> _bef;
    view<I> _aft;
  };

  template <range R, typename T>
  constexpr auto around(R &&r, const T &sep) -> decltype(auto)
  {
    auto pos = find(forward<R>(r), sep);

    if (pos != end(forward<R>(r)))
    {
      return around_pair<decltype(begin(forward<R>(r)))>{
          true,
          {begin(forward<R>(r)), pos},
          {pos + 2, end(forward<R>(r))}};
    }
    else
    {
      return around_pair<decltype(begin(forward<R>(r)))>{
          false,
          {begin(forward<R>(r)), begin(forward<R>(r))},
          {begin(forward<R>(r)), end(forward<R>(r))}};
    }
  }

  template <range R1, predicate<range_const_reference<R1>> P>
  constexpr bool all_of(const R1 &r, P &&p)
  {
    for (const auto &i : r)
    {
      if (!p(i))
      {
        return false;
      }
    }

    return true;
  }

  template <range R1, predicate<range_const_reference<R1>> P>
  constexpr bool any_of(const R1 &r, P &&p)
  {
    for (const auto &i : r)
    {
      if (p(i))
      {
        return true;
      }
    }

    return false;
  }

  template <range R1, predicate<range_const_reference<R1>> P>
  constexpr bool none_of(const R1 &r, P &&p)
  {
    for (const auto &i : r)
    {
      if (p(i))
      {
        return false;
      }
    }

    return true;
  }

  template <range R, typename I>
  constexpr void copy(R &&r, I i)
  {
    auto b = begin(forward<R>(r));
    auto e = end(forward<R>(r));

    while (b != e)
    { 
      *i = *b;
      ++i;
      ++b;
    }
  }

  //----------------------------
  //
  // Iterator utilities
  //
  //----------------------------

  template <integral I = size_t>
  class incremental_iterator
  {
  private:
    I _current = 0;
    I _step = 1;

  public:
    constexpr ~incremental_iterator() = default;
    constexpr incremental_iterator() = delete;
    constexpr incremental_iterator(I current, I step) : _current(current), _step(step) {}
    constexpr incremental_iterator(const incremental_iterator &) = default;
    constexpr incremental_iterator(incremental_iterator &&) = default;
    constexpr incremental_iterator &operator=(const incremental_iterator &) = default;
    constexpr incremental_iterator &operator=(incremental_iterator &&) = default;

  public:
    constexpr auto operator==(const incremental_iterator &o) const
    {
      return _current == o._current;
    }

    constexpr auto operator!=(const incremental_iterator &o) const
    {
      return !(*this == o);
    }

    constexpr incremental_iterator &operator++()
    {
      _current += _step;
      return *this;
    }

    constexpr incremental_iterator operator++(int)
    {
      auto copy = *this;
      ++*this;
      return copy;
    }

    constexpr auto operator*()
    {
      return _current;
    }

    constexpr auto operator-(const incremental_iterator &o) const
    {
      return _current - o._current;
    }
  };

  template <integral I>
  constexpr view<incremental_iterator<I>> upto(I from, I to, I step = 1)
  {
    assert(from <= to);
    assert((to - from) % step == 0);
    return view<incremental_iterator<I>>(
        incremental_iterator<I>(from, step),
        incremental_iterator<I>(to, step));
  }

  template <integral I = size_t>
  class decremental_iterator
  {
  private:
    I _current = 0;
    I _step = 1;

  public:
    constexpr ~decremental_iterator() = default;
    constexpr decremental_iterator() = delete;
    constexpr decremental_iterator(I current, I step) : _current(current), _step(step) {}
    constexpr decremental_iterator(const decremental_iterator &) = default;
    constexpr decremental_iterator(decremental_iterator &&) = default;
    constexpr decremental_iterator &operator=(const decremental_iterator &) = default;
    constexpr decremental_iterator &operator=(decremental_iterator &&) = default;

  public:
    constexpr auto operator==(const decremental_iterator &o) const
    {
      return _current == o._current;
    }

    constexpr auto operator!=(const decremental_iterator &o) const
    {
      return !(*this == o);
    }

    constexpr decremental_iterator &operator++()
    {
      _current -= _step;
      return *this;
    }

    constexpr decremental_iterator operator++(int)
    {
      auto copy = *this;
      ++*this;
      return copy;
    }

    constexpr auto operator*()
    {
      return _current;
    }

    constexpr auto operator-(const decremental_iterator &o) const
    {
      return _current - o._current;
    }
  };

  template <integral I>
  constexpr view<decremental_iterator<I>> downto(I from, I to, I step = 1)
  {
    assert(from >= to);
    assert((from - to) % step == 0);
    return view<decremental_iterator<I>>(
        decremental_iterator<I>(from, step),
        decremental_iterator<I>(to, step));
  }

  template <push_back_range R>
  class push_back_iterator
  {
  private:
    R *_range = nullptr;

  public:
    constexpr push_back_iterator(R &range) : _range(&range)
    {
    }

    template <typename T>
    push_back_iterator &operator=(T &&t)
    {
      _range->push_back(forward<T>(t));
      return *this;
    }

    push_back_iterator &operator*()
    {
      return *this;
    }

    push_back_iterator &operator++()
    {
      return *this;
    }

    push_back_iterator &operator++(int)
    {
      return *this;
    }

    auto operator==(const push_back_iterator &o)
    {
      return true;
    }
  };

  template <push_back_range C>
  push_back_iterator<C> push_back_inserter(C &c)
  {
    return push_back_iterator<C>(c);
  }

  template <push_range R>
  class push_iterator
  {
  private:
    R *_range = nullptr;

  public:
    constexpr push_iterator(R &range) : _range(&range)
    {
    }

    template <typename T>
    push_iterator &operator=(T &&t)
    {
      _range->push(forward<T>(t));
      return *this;
    }

    push_iterator &operator*()
    {
      return *this;
    }

    push_iterator &operator++()
    {
      return *this;
    }

    push_iterator &operator++(int)
    {
      return *this;
    }

    auto operator==(const push_iterator &o)
    {
      return true;
    }
  };

  template <push_range C>
  push_iterator<C> push_inserter(C &c)
  {
    return push_iterator<C>(c);
  }

  // ---------------------------------
  //
  // Containers
  //
  // ---------------------------------

  template <class T, size_t N>
  struct array
  {
    T _data[N];

  public:
    constexpr auto size() const
    {
      return N;
    }

    constexpr auto begin()
    {
      return _data;
    }

    constexpr auto end()
    {
      return _data + N;
    }

    constexpr auto begin() const
    {
      return _data;
    }

    constexpr auto end() const
    {
      return _data + N;
    }

    constexpr T &operator[](size_t i)
    {
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      return _data[i];
    }
  };

  template <size_t I, typename T, size_t N>
  constexpr auto get(array<T, N> &a) -> decltype(auto)
  {
    return a[I];
  }

  template <size_t I, typename T, size_t N>
  constexpr auto get(const array<T, N> &a) -> decltype(auto)
  {
    return a[I];
  }

  template <size_t I, typename T, size_t N>
  constexpr auto get(array<T, N> &&a) -> decltype(auto)
  {
    return a[I];
  }

  template <size_t I, typename T, size_t N>
  constexpr auto get(const array<T, N> &&a) -> decltype(auto)
  {
    return a[I];
  }

  template <typename T, size_t N>
  class static_stack
  {
  private:
    array<T, N> _data;
    size_t _idx = N;

  public:
    constexpr ~static_stack() = default;
    constexpr static_stack() = default;

    template <range R>
    constexpr static_stack(R &&r)
    {
      push_back(forward<R>(r));
    }

    constexpr static_stack(const static_stack &) = default;
    constexpr static_stack(static_stack &&) = default;
    constexpr static_stack &operator=(const static_stack &) = default;
    constexpr static_stack &operator=(static_stack &&) = default;

    template <range R>
    constexpr static_stack &operator=(R &&r)
    {
      return (*this = transfer(static_stack(forward<R>(r))));
    }

  public:
    constexpr auto size() const
    {
      return N - _idx;
    }

    constexpr auto empty() const
    {
      return _idx == N;
    }

    constexpr auto full() const
    {
      return _idx == 0;
    }

  public:
    constexpr auto begin()
    {
      return _data.begin() + _idx;
    }

    constexpr auto end()
    {
      return _data.end();
    }

    constexpr auto begin() const
    {
      return _data.begin() + _idx;
    }

    constexpr auto end() const
    {
      return _data.end();
    }

  public:
    template <convertible_to<T> U>
    constexpr void push(U &&u)
    {
      if (!full())
      {
        _data[--_idx] = forward<U>(u);
      }
    }

    template <range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter(*this));
    }

    constexpr T &pop()
    {
      return _data[_idx++];
    }
  };

  template <typename T>
  class fixed_stack
  {
  private:
    size_t _max{0};
    size_t _idx{_max};
    owning<T[]> _data;

  public:
    constexpr ~fixed_stack() = default;
    constexpr fixed_stack() = default;

    constexpr fixed_stack(size_t max)
        : _max(max), _idx(max), _data(new T[max]) {}

    template <range R>
    constexpr fixed_stack(R &&r)
        : fixed_stack(stew::end(r) - stew::begin(r))
    {
      push(forward<R>(r));
    }

    constexpr fixed_stack(const fixed_stack &) = default;
    constexpr fixed_stack(fixed_stack &&) = default;
    constexpr fixed_stack &operator=(const fixed_stack &) = default;
    constexpr fixed_stack &operator=(fixed_stack &&) = default;

    template <range R>
    constexpr fixed_stack &operator=(R &&r)
    {
      return (*this = transfer(fixed_stack(forward<R>(r))));
    }

  public:
    constexpr auto size() const
    {
      return _max - _idx;
    }

    constexpr auto empty() const
    {
      return _idx == _max;
    }

    constexpr auto full() const
    {
      return _idx == 0;
    }

  public:
    constexpr auto begin()
    {
      return _data.get() + _idx;
    }

    constexpr auto end()
    {
      return _data.get() + _max;
    }

    constexpr auto begin() const
    {
      return _data.get() + _idx;
    }

    constexpr auto end() const
    {
      return _data.get() + _max;
    }

  public:
    template <convertible_to<T> U>
    constexpr void push(U &&u)
    {
      if (!full())
      {
        _data[--_idx] = forward<U>(u);
      }
    }

    template <range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter(*this));
    }

    constexpr T &pop()
    {
      return _data[_idx++];
    }
  };

  template <typename T>
  class stack
  {
  private:
    fixed_stack<T> _data;

  public:
    constexpr ~stack() = default;
    constexpr stack() = default;
    constexpr stack(size_t max) : _data(max) {}
    template <range R>
    constexpr stack(R &&r)
        : stack(stew::end(r) - stew::begin(r))
    {
      push_back(forward<R>(r));
    }

    constexpr stack(const stack &) = default;
    constexpr stack(stack &&) = default;

    constexpr stack &operator=(const stack &) = default;
    constexpr stack &operator=(stack &&) = default;

    template <range R>
    constexpr stack &operator=(R &&r)
    {
      return (*this = transfer(stack(forward<R>(r))));
    }

  public:
    constexpr auto size() const
    {
      return _data.size();
    }

    constexpr auto empty() const
    {
      return _data.empty();
    }

    constexpr auto full() const
    {
      return _data.full();
    }

  public:
    constexpr auto begin()
    {
      return _data.begin();
    }

    constexpr auto end()
    {
      return _data.end();
    }

    constexpr auto begin() const
    {
      return _data.begin();
    }

    constexpr auto end() const
    {
      return _data.end();
    }

  public:
    template <convertible_to<T> U>
    constexpr void push(U &&u)
    {
      if (_data.full())
      {
        fixed_stack<T> tmp(_data.size());
        tmp.push(_data);
        _data = fixed_stack<T>(tmp.size() * 2 + 10);
        _data.push(transfer(tmp));
      }

      _data.push(forward<U>(u));
    }

    template <range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter(*this));
    }

    constexpr T &pop()
    {
      return _data.pop();
    }
  };

  template <typename T, size_t N>
  class static_vector
  {
  private:
    array<T, N> _data;
    size_t _size = 0;

  public:
    constexpr ~static_vector() = default;
    constexpr static_vector() = default;

    template <range R>
    constexpr static_vector(R &&r)
    {
      push_back(forward<R>(r));
    }

    constexpr static_vector(const static_vector &) = default;
    constexpr static_vector(static_vector &&) = default;
    constexpr static_vector &operator=(const static_vector &) = default;
    constexpr static_vector &operator=(static_vector &&) = default;

    template <range R>
    constexpr static_vector &operator=(R &&r)
    {
      return (*this = transfer(static_vector(forward<R>(r))));
    }

  public:
    constexpr auto size() const
    {
      return _size;
    }

    constexpr auto empty() const
    {
      return _size == 0;
    }

    constexpr auto full() const
    {
      return _size == N;
    }

  public:
    constexpr auto begin()
    {
      return _data.begin();
    }

    constexpr auto end()
    {
      return _data.begin() + _size;
    }

    constexpr auto begin() const
    {
      return _data.begin();
    }

    constexpr auto end() const
    {
      return _data.begin() + _size;
    }

    constexpr T &operator[](size_t i)
    {
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      return _data[i];
    }

  public:
    template <convertible_to<T> U>
    constexpr void push_back(U &&u)
    {
      if (!full())
      {
        _data[_size] = forward<U>(u);
        ++_size;
      }
    }

    template <range R>
    constexpr void push_back(R &&r)
    {
      copy(forward<R>(r), push_back_inserter(*this));
    }
  };

  template <typename T>
  class fixed_vector
  {
  private:
    size_t _size{0};
    size_t _max{0};
    owning<T[]> _data;

  public:
    constexpr ~fixed_vector() = default;
    constexpr fixed_vector() = default;

    constexpr fixed_vector(size_t max)
        : _size{0}, _max{max},
          _data{new T[_max]}
    {
    }

    constexpr fixed_vector(const fixed_vector &o)
        : fixed_vector(o._max)
    {
      push_back(o);
    }

    constexpr fixed_vector(fixed_vector &&o)
        : _size{transfer(o._size)},
          _max{transfer(o._max)},
          _data{transfer(o._data)}
    {
    }

    template <range R>
    constexpr fixed_vector(R &&r)
        : fixed_vector(stew::end(forward<R>(r)) -
                       stew::begin(forward<R>(r)))
    {
      push_back(forward<R>(r));
    }

    constexpr fixed_vector &operator=(fixed_vector o)
    {
      _size = transfer(o._size);
      _max = transfer(o._max);
      _data = transfer(o._data);
      return *this;
    }

    template <range R>
    constexpr fixed_vector &operator=(R &&r)
    {
      return (*this = transfer(fixed_vector(forward<R>(r))));
    }

  public:
    constexpr auto begin()
    {
      return _data.get();
    }

    constexpr auto end()
    {
      return begin() + _size;
    }

    constexpr auto begin() const
    {
      return _data.get();
    }

    constexpr auto end() const
    {
      return begin() + _size;
    }

    constexpr T &operator[](size_t i)
    {
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      return _data[i];
    }

  public:
    constexpr bool empty() const
    {
      return _size == 0;
    }

    constexpr bool full() const
    {
      return _size == _max;
    }

    constexpr auto size() const
    {
      return _size;
    }

  public:
    template <convertible_to<T> U>
    constexpr void push_back(U &&u)
    {
      if (!full())
      {
        _data[_size] = forward<U>(u);
        _size += 1;
      }
    }

    template <range R>
    constexpr void push_back(R &&r)
    {
      copy(forward<R>(r), push_back_inserter(*this));
    }

    constexpr void pop_back()
    {
      if (_size != 0)
      {
        _size -= 1;
      }
    }
  };

  template <typename T>
  class vector
  {
  private:
    fixed_vector<T> _data;

  public:
    constexpr ~vector() = default;
    constexpr vector() = default;
    constexpr vector(size_t max) : _data(max) {}

    template <range R>
    constexpr vector(R &&r)
        : vector(stew::end(forward<R>(r)) -
                 stew::begin(forward<R>(r)))
    {
      push_back(forward<R>(r));
    }

    constexpr vector(const vector &) = default;
    constexpr vector(vector &) = default;
    constexpr vector &operator=(const vector &) = default;
    constexpr vector &operator=(vector &&) = default;

    template <range R>
    constexpr vector &operator=(R &&r)
    {
      return (*this = transfer(vector(forward<R>(r))));
    }

  public:
    constexpr auto begin()
    {
      return _data.begin();
    }

    constexpr auto end()
    {
      return _data.end();
    }

    constexpr auto begin() const
    {
      return _data.begin();
    }

    constexpr auto end() const
    {
      return _data.end();
    }

    constexpr T &operator[](size_t i)
    {
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      return _data[i];
    }

  public:
    constexpr auto size() const
    {
      return _data.size();
    }

    constexpr auto empty() const
    {
      return _data.empty();
    }

    constexpr auto full() const
    {
      return _data.full();
    }

  public:
    template <convertible_to<T> U>
    constexpr void push_back(U &&u)
    {
      if (_data.full())
      {
        fixed_vector<T> tmp = transfer(_data);
        _data = fixed_vector<T>(tmp.size() * 2 + 10);
        _data.push_back(transfer(tmp));
      }

      _data.push_back(forward<U>(u));
    }

    template <range R>
    constexpr void push_back(R &&r)
    {
      copy(forward<R>(r), push_back_inserter(*this));
    }

    constexpr void pop_back()
    {
      _data.pop_back();
    }
  };

  namespace todo
  {
    template <typename T>
    class list
    {
      struct node
      {
        T _t;
        size_t _next = static_cast<size_t>(-1);

      public:
        ~node() = default;
        node() = default;
        template <convertible_to<T> U>
        node(U &&u) : _t(forward<U>(u)) {}
        node(const node &o) : _t(o._t), _next(o._next) {}
        node(node &&o) : _t(transfer(o._t)), _next(transfer(o._next))
        {
          o._next = static_cast<size_t>(-1);
        }

        node &operator=(node o)
        {
          _t = transfer(o._t);
          _next = transfer(o._next);
          return *this;
        }
      };

      struct iterator
      {
        non_owning<list> _l;
        size_t _current = static_cast<size_t>(-1);

        iterator &operator++()
        {
          if (_current != static_cast<size_t>(-1))
          {
            _current = _l->_nodes[_current]._next;
          }

          return *this;
        }

        iterator operator++(int)
        {
          iterator copy = *this;
          ++this;
          return copy;
        }

        bool operator==(const iterator &o) const
        {
          return _l == o._l && _current == o._current;
        }

        bool operator!=(const iterator &o) const
        {
          return !(*this == o);
        }

        T &operator*() const
        {
          return _l->_nodes[_current]._t;
        }
      };

      struct const_iterator
      {
        non_owning<const list> _l;
        size_t _current = static_cast<size_t>(-1);

        const_iterator &operator++()
        {
          if (_current != static_cast<size_t>(-1))
          {
            _current = _l->_nodes[_current]._next;
          }

          return *this;
        }

        const_iterator operator++(int)
        {
          iterator copy = *this;
          ++this;
          return copy;
        }

        bool operator==(const const_iterator &o) const
        {
          return _l == o._l && _current == o._current;
        }

        bool operator!=(const const_iterator &o) const
        {
          return !(*this == o);
        }

        const T &operator*() const
        {
          return _l->_nodes[_current]._t;
        }
      };

    private:
      vector<node> _nodes;
      size_t _first = static_cast<size_t>(-1);
      size_t _last = static_cast<size_t>(-1);

    public:
      ~list() = default;
      list() = default;
      list(const list &o) = default;
      list &operator=(const list &) = default;
      list &operator=(list &&) = default;

    public:
      template <convertible_to<T> U>
      void push_back(U &&u)
      {
        _nodes.push_back(node(forward<U>(u)));

        if (_last == static_cast<size_t>(-1))
        {
          _first = 0;
          _last = 0;
        }
        else
        {
          _nodes[_last]._next = _nodes.size() - 1;
          _last = _nodes.size() - 1;
        }
      }

      template <range R>
      void push_back(R &&r)
      {
        for (auto &&i : forward<R>(r))
        {
          push_back(forward<decltype(i)>(i));
        }
      }

      template <convertible_to<T> U>
      void push_front(U &&u)
      {
        _nodes.push_back(node(forward<U>(u)));

        if (_first == static_cast<size_t>(-1))
        {
          _first = 0;
          _last = 0;
        }
        else
        {
          _nodes[_nodes.size() - 1]._next = _first;
          _first = _nodes.size() - 1;
        }
      }

      template <range R>
      void push_front(R &&r)
      {
        for (auto &&i : forward<R>(r))
        {
          push_front(forward<decltype(i)>(i));
        }
      }

      template <typename I, convertible_to<T> U>
        requires same_one_of<I, iterator, const_iterator>
      void insert(I i, U &&u)
      {
        // TODO: faire la fonction insert
      }

    public:
      auto size() const
      {
        return _nodes.size();
      }

      bool empty() const
      {
        return _nodes.empty();
      }

    public:
      auto begin()
      {
        return iterator{this, _first};
      }

      auto end()
      {
        return iterator{this, static_cast<size_t>(-1)};
      }

      auto begin() const
      {
        return const_iterator{this, _first};
      }

      auto end() const
      {
        return const_iterator{this, static_cast<size_t>(-1)};
      }
    };

    template <typename T>
    class set
    {
    private:
      vector<T> _data;

    public:
      ~set() = default;
      set() = default;
      set(const set &) = default;
      set(set &&) = default;
      set &operator=(const set &) = default;
      set &operator=(set &&) = default;

    public:
      auto size() const
      {
        return _data.size();
      }

      auto empty() const
      {
        return _data.empty();
      }

    public:
      auto begin()
      {
        return _data.begin();
      }

      auto end()
      {
        return _data.end();
      }

      auto begin() const
      {
        return _data.begin();
      }

      auto end() const
      {
        return _data.end();
      }

    public:
      template <convertible_to<T> U>
      void push(U &&u)
      {
        if (find(_data, forward<U>(u)) == _data.end())
        {
          _data.push_back(forward<U>(u));
        }
      }

      template <convertible_to<T> U>
      void pop(const U &u)
      {
      }
    };
  }

  //----------------------------
  //
  // String classes
  //
  //----------------------------

  template <character C>
  using basic_string_view = view<const C *>;

  using string_view = basic_string_view<char>;
  using wstring_view = basic_string_view<wchar_t>;

  constexpr string_view operator"" _sv(const char *s, size_t n)
  {
    return string_view(s, s + n);
  }

  constexpr wstring_view operator"" _sv(const wchar_t *s, size_t n)
  {
    return wstring_view(s, s + n);
  }

  template <typename S, typename C>
  concept string_view_like =
      character<C> && convertible_to<S, basic_string_view<C>>;

  template <character C>
  using basic_fixed_string = fixed_vector<C>;

  using fstring = basic_fixed_string<char>;
  using wfstring = basic_fixed_string<wchar_t>;

  template <character C>
  using basic_string = vector<C>;

  using string = basic_string<char>;
  using wstring = basic_string<wchar_t>;

  string operator"" _s(const char *s, size_t n)
  {
    return string(string_view(s, s + n));
  }

  wstring operator"" _s(const wchar_t *s, size_t n)
  {
    return wstring(wstring_view(s, s + n));
  }

  //------------------------------
  //
  // Formatting
  //
  //------------------------------

  template <typename O>
  concept char_ostream =
      requires(O &o, char c, string_view s) {
        o.push_back(c);
        o.push_back(s);
      };

  template <typename O>
  concept wchar_ostream =
      requires(O &o, wchar_t c, wstring_view s) {
        o.push_back(c);
        o.push_back(s);
      };

  template <character C, size_t N>
  class basic_format_string
  {
  private:
    array<basic_string_view<C>, N> _items;

  public:
    consteval basic_format_string(basic_string_view<C> fmt)
        : _items(split(fmt))
    {
    }

    template <size_t M>
    consteval basic_format_string(const C (&fmt)[M])
        : basic_format_string(basic_string_view<C>(fmt))
    {
    }

    constexpr const array<basic_string_view<C>, N> &
    items() const
    {
      return _items;
    }

  private:
    consteval array<basic_string_view<C>, N>
    split(basic_string_view<C> fmt) const
    {
      array<basic_string_view<C>, N> parts;

      for (auto &part : parts)
      {
        auto [found, bef, aft] = around(fmt, "{}"_sv);

        if (found)
        {
          fmt = aft;
          part = bef;
        }
        else
        {
          part = aft;
          break;
        }
      }

      return parts;
    }
  };

  template <typename O>
  concept ostream = char_ostream<O> || wchar_ostream<O>;

  template <typename T>
  class formatter;

  namespace fmt
  {
    template <size_t I, size_t N, ostream O, character C, typename H, typename... T>
    constexpr void format_to_one(
        O &o, const basic_format_string<C, N> &fmt, const H &h, const T &...t)
    {
      formatter<basic_string_view<C>>::to(o, get<I>(fmt.items()));
      formatter<H>::to(o, h);

      if constexpr (sizeof...(T) > 0)
      {
        format_to_one<I + 1>(o, fmt, t...);
      }
    }

    template <ostream O, character C, typename H, typename... T>
    constexpr void format_to(
        O &o, const basic_format_string<C, sizeof...(T) + 2> &fmt,
        const H &h, const T &...t)
    {
      format_to_one<0>(o, fmt, h, t...);
      formatter<basic_string_view<C>>::to(o, get<sizeof...(T) + 1>(fmt.items()));
    }

    template <ostream O, character C>
    constexpr void format_to(O &o, const basic_format_string<C, 1> &fmt)
    {
      formatter<basic_string_view<C>>::to(o, get<0>(fmt.items()));
    }
  }

  template <ostream O, typename... A>
  constexpr void format_to(O &o, const basic_format_string<char, sizeof...(A) + 1> &fmt, const A &...a)
  {
    fmt::format_to(o, fmt, a...);
  }

  template <ostream O, typename... A>
  constexpr void format_to(O &o, const basic_format_string<wchar_t, sizeof...(A) + 1> &fmt, const A &...a)
  {
    fmt::format_to(o, fmt, a...);
  }

  template <character C>
  class formatter<C>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, C o)
    {
      os.push_back(o);
    }
  };

  template <character C>
  class formatter<basic_string_view<C>>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, basic_string_view<C> o)
    {
      os.push_back(o);
    }
  };

  template <character C, size_t N>
  class formatter<const C (&)[N]>
      : public formatter<basic_string_view<C>>
  {
  };

  template <character C, size_t N>
  class formatter<C (&)[N]>
      : public formatter<basic_string_view<C>>
  {
  };

  template <character C, size_t N>
  class formatter<const C[N]>
      : public formatter<basic_string_view<C>>
  {
  };

  template <character C, size_t N>
  class formatter<C[N]>
      : public formatter<basic_string_view<C>>
  {
  };

  template <character C>
  class formatter<basic_fixed_string<C>>
      : public formatter<basic_string_view<C>>
  {
  };

  template <character C>
  class formatter<basic_string<C>>
      : public formatter<basic_string_view<C>>
  {
  };

  template <signed_integral I>
  class formatter<I>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, I i)
    {
      bool neg = i < 0;

      I tmp = neg ? -i : i;

      static_stack<char, 20> tbuff;

      if (tmp == 0)
      {
        formatter<char>::to(o, '0');
      }
      else
      {
        while (tmp != 0)
        {
          tbuff.push("0123456789"[tmp % 10]);
          tmp /= 10;
        }
      }

      if (neg)
      {
        formatter<char>::to(o, '-');
      }

      formatter<basic_string_view<char>>::to(o, tbuff);
    }
  };

  template <unsigned_integral I>
  class formatter<I>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, I i)
    {
      static_stack<char, 20> tbuff;
      I tmp = i;

      if (tmp == 0)
      {
        formatter<char>::to(o, '0');
      }
      else
      {
        while (tmp != 0)
        {
          tbuff.push("0123456789"[tmp % 10]);
          tmp /= 10;
        }
      }

      formatter<basic_string_view<char>>::to(o, tbuff);
    }
  };

  template <>
  class formatter<bool>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, bool b)
    {
      formatter<basic_string_view<char>>::to(o, b ? "true"_sv : "false"_sv);
    }
  };

  template <pointer P>
  class formatter<P>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, P p)
    {
      formatter<size_t>::to(os, (size_t)(void *)(p));
    }
  };

  //------------------------
  //
  // Multithreading
  //
  //------------------------

  class thread
  {
  private:
    thrd_t _thrid;
    bool _joinable = false;

  private:
    template <typename F>
    static int fcall_wrapper(void *ctx)
    {
      (*static_cast<F *>(ctx))();
      return 0;
    }

  public:
    ~thread() = default;

    template <typename F>
    thread(F &&f)
    {
      auto tmp = forward<F>(f);

      if (thrd_create(&_thrid, fcall_wrapper<F>, &tmp) == thrd_success)
      {
        _joinable = true;
      }
    }

    thread(const thread &) = delete;
    thread(thread &&) = default;
    thread &operator=(const thread &) = delete;
    thread &operator=(thread &&) = default;

  public:
    void join()
    {
      if (_joinable && thrd_join(_thrid, nullptr) == thrd_success)
      {
        _joinable = false;
      }
    }

    void detach()
    {
      if (_joinable && thrd_detach(_thrid) == thrd_success)
      {
        _joinable = false;
      }
    }

    bool operator==(thrd_t id) const
    {
      return thrd_equal(_thrid, id) == 0;
    }

    bool operator==(const thread &o) const
    {
      return thrd_equal(_thrid, o._thrid) == 0;
    }
  };

  class jthread
  {
  private:
    thread _t;

  public:
    ~jthread()
    {
      _t.join();
    }

    template <typename F>
    jthread(F &&f) : _t(forward<F>(f)) {}
    jthread(const jthread &) = delete;
    jthread(jthread &&) = default;
    jthread &operator=(const jthread &) = delete;
    jthread &operator=(jthread &&) = default;

  public:
    void join()
    {
      _t.join();
    }

    void detach()
    {
      _t.detach();
    }

    bool operator==(thrd_t id) const
    {
      return _t.operator==(id);
    }

    bool operator==(const thread &o) const
    {
      return _t == o;
    }

    bool operator==(const jthread &o) const
    {
      return _t == o._t;
    }
  };

  class dthread
  {
  private:
    thread _t;

  public:
    ~dthread() = default;
    template <typename F>
    dthread(F &&f) : _t(forward<F>(f)) { detach(); }
    dthread(const dthread &) = delete;
    dthread(dthread &&) = default;
    dthread &operator=(const dthread &) = delete;
    dthread &operator=(dthread &&) = default;

  public:
    void join()
    {
      _t.join();
    }

    void detach()
    {
      _t.detach();
    }

    bool operator==(thrd_t id) const
    {
      return _t.operator==(id);
    }

    bool operator==(const thread &o) const
    {
      return _t == o;
    }

    bool operator==(const dthread &o) const
    {
      return _t == o._t;
    }
  };

  template <typename T>
  class future
  {
    function<T()> _func;

  public:
    ~future() = default;
    future() = default;

    template <typename F>
    future(F &&func) : _func(forward<F>(func))
    {
    }

    future(const future &) = default;
    future(future &&) = default;
    future &operator=(const future &) = default;
    future &operator=(future &&) = default;

  public:
    T get()
    {
      return _func();
    }
  };

  template <typename F, typename... A>
  auto defer(F &&f, A &&...args) -> decltype(auto)
  {
    using res_t = decltype(forward<F>(f)(forward<A>(args)...));

    return future<res_t>([&]
                         { return forward<F>(f)(forward<A>(args)...); });
  }

  template <typename F, typename... A>
  auto async(F &&f, A &&...args) -> decltype(auto)
  {
    using res_t = decltype(forward<F>(f)(forward<A>(args)...));

    return future<res_t>([&]
                         {
      if constexpr (!same_as<res_t, void>) 
      {
        res_t res;
        thread([&] { res = forward<F>(f)(forward<A>(args)...);}).join();
        return res;
      }
      else 
      {
        jthread([&] { forward<F>(f)(forward<A>(args)...);});
      } });
  }

  enum class duration_type
  {
    second,
    millisecond,
    microsecond,
    nanosecond
  };

  template <duration_type T>
  class duration_spec
  {
    timespec _time;

  public:
    constexpr ~duration_spec() = default;
    constexpr duration_spec() = default;
    constexpr duration_spec(long time) { _spec(time); }
    constexpr duration_spec(const duration_spec &) = default;
    constexpr duration_spec(duration_spec &&) = default;
    constexpr duration_spec &operator=(const duration_spec &) = default;
    constexpr duration_spec &operator=(duration_spec &&) = default;

  private:
    void _spec(long time)
    {
      switch (T)
      {
      case duration_type::second:
        _time.tv_sec = time;
        break;
      case duration_type::millisecond:
        _time.tv_nsec = 1'000'000 * time;
        break;
      case duration_type::microsecond:
        _time.tv_nsec = 1'000 * time;
        break;
      case duration_type::nanosecond:
        _time.tv_nsec = time;
        break;
      }
    }

  public:
    constexpr auto &spec() const
    {
      return (_time);
    }
  };

  namespace this_thread
  {
    template <duration_type T>
    inline void sleep(duration_spec<T> spec)
    {
      thrd_sleep(&spec.spec(), nullptr);
    }

    inline void yield()
    {
      thrd_yield();
    }

    inline void exit(int exitcode)
    {
      thrd_exit(exitcode);
    }

    inline auto id()
    {
      return thrd_current();
    }
  }

  enum class mutex_type : int
  {
    plain = mtx_plain,
    timed = mtx_timed,
    recursive = mtx_plain | mtx_recursive,
    timed_recursive = mtx_timed | mtx_recursive
  };

  template <mutex_type T>
  concept is_timed = (T == mutex_type::timed) ||
                     (T == mutex_type::timed_recursive);

  template <mutex_type T>
  class basic_mutex
  {
  private:
    using mt = mutex_type;

  private:
    mtx_t _m;
    bool _lockable = false;

  public:
    ~basic_mutex()
    {
      mtx_destroy(&_m);
    }

    basic_mutex()
        : _lockable(mtx_init(&_m, static_cast<int>(T)) == thrd_success)
    {
    }

    basic_mutex(const basic_mutex &) = delete;
    basic_mutex(basic_mutex &&) = default;
    basic_mutex &operator=(const basic_mutex &) = delete;
    basic_mutex &operator=(basic_mutex &&) = default;

  public:
    auto underlying()
    {
      return &_m;
    }

    void lock()
    {
      if (_lockable)
      {
        mtx_lock(&_m);
      }
    }

    void trylock()
    {
      if (_lockable)
      {
        mtx_trylock(&_m);
      }
    }

    template <duration_type Tp>
    void timedlock(duration_spec<Tp> spec)
      requires is_timed<T>
    {
      if (_lockable)
      {
        mtx_timedlock(&_m, &spec.spec());
      }
    }

    void unlock()
    {
      if (_lockable)
      {
        mtx_unlock(&_m);
      }
    }
  };

  using mutex = basic_mutex<mutex_type::plain>;
  using timed_mutex = basic_mutex<mutex_type::timed>;
  using recursive_mutex = basic_mutex<mutex_type::recursive>;
  using timed_recursive_mutex = basic_mutex<mutex_type::timed_recursive>;

  template <mutex_type T>
  class basic_scoped_lock
  {
  private:
    reference<basic_mutex<T>> _m;

  public:
    ~basic_scoped_lock()
    {
      _m.get().unlock();
    }

    basic_scoped_lock(basic_mutex<T> &m) : _m(m)
    {
      _m.get().lock();
    }

    basic_scoped_lock(const basic_scoped_lock &) = delete;
    basic_scoped_lock(basic_scoped_lock &&) = delete;
    basic_scoped_lock &operator=(const basic_scoped_lock &) = delete;
    basic_scoped_lock &operator=(basic_scoped_lock &&) = delete;
  };

  using scoped_lock = basic_scoped_lock<mutex_type::plain>;
  using timed_scoped_lock = basic_scoped_lock<mutex_type::timed>;
  using recursive_scoped_lock = basic_scoped_lock<mutex_type::recursive>;
  using timed_recursive_scoped_lock = basic_scoped_lock<mutex_type::timed_recursive>;

  class condition_variable
  {
  private:
    cnd_t _c;
    bool _useable = false;

  public:
    ~condition_variable()
    {
      if (_useable)
      {
        cnd_destroy(&_c);
      }
    }

    condition_variable()
        : _useable(cnd_init(&_c) == thrd_success)
    {
    }

    condition_variable(const condition_variable &) = default;
    condition_variable(condition_variable &&) = default;
    condition_variable &operator=(const condition_variable &) = default;
    condition_variable &operator=(condition_variable &&) = default;

  public:
    void notify_one()
    {
      if (_useable)
      {
        cnd_signal(&_c);
      }
    }

    void notify_all()
    {
      if (_useable)
      {
        cnd_broadcast(&_c);
      }
    }

    template <mutex_type T>
    void wait(basic_mutex<T> &m)
    {
      if (_useable)
      {
        cnd_wait(&_c, m.underlying());
      }
    }

    template <mutex_type T, typename P>
    void wait(basic_mutex<T> &m, P &&pred)
    {
      if (_useable)
      {
        while (!forward<P>(pred)())
        {
          wait(m);
        }
      }
    }

    template <mutex_type T, duration_type Tp>
    void waitfor(basic_mutex<T> &m, duration_spec<Tp> spec)
    {
      if (_useable)
      {
        cnd_timedwait(&_c, m.underlying(), &spec.spec());
      }
    }
  };

  //----------------------
  //
  // Ostream
  //
  //----------------------

  template <character C>
  class basic_fostream
  {
  private:
    FILE *_out = nullptr;

  public:
    ~basic_fostream()
    {
      if (_out != nullptr)
      {
        fclose(_out);
        _out = nullptr;
      }
    }

    basic_fostream() = default;

    basic_fostream(const C *path)
        : _out(fopen(path, "w"))
    {
    }

    basic_fostream(FILE *o)
        : _out(o)
    {
    }

    basic_fostream(const basic_fostream &) = delete;
    basic_fostream(basic_fostream &&) = default;
    basic_fostream &operator=(const basic_fostream &) = delete;
    basic_fostream &operator=(basic_fostream &) = default;

  public:
    void push_back(C c)
    {
      putc(c, _out);
    }

    void push_back(basic_string_view<C> s)
    {
      fwrite(basic_string_view<C>(s).begin(), sizeof(C), basic_string_view<C>(s).size(), _out);
    }

    void flush()
    {
      fflush(_out);
    }

    void close()
    {
      fclose(_out);
      _out = nullptr;
    }

  public:
    template <typename... T>
    void printf(const basic_format_string<C, sizeof...(T) + 1> &fmt, const T &...t)
    {
      format_to(*this, fmt, t...);
    }

    template <typename... T>
    void printfln(const basic_format_string<C, sizeof...(T) + 1> &fmt, const T &...t)
    {
      printf(fmt, t...);
      push_back('\n');
    }
  };

  using fostream = basic_fostream<char>;
  using wfostream = basic_fostream<wchar_t>;

  fostream cout(stdout);
  fostream cerr(stderr);

}

#endif
