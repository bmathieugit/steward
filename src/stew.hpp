#ifndef __stew_hpp__
#define __stew_hpp__

#include <stdio.h>
#include <threads.h>
#include <time.h>
#include <assert.h>

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

  //--------------------------------
  //
  // Meta : add and remove on T
  //
  //--------------------------------

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

    template <typename T, size_t N>
    struct rm_array<T[N]> : struct_type<T>
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
  using rm_cv = rm_const<rm_volatile<T>>;

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

  template <typename T>
  using strictly = rm_cvref<T>;

  //-----------------------------------
  //
  // Meta : generals concepts
  //
  //-----------------------------------

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

  template <typename From, typename To>
  concept convertible_to =
      (same_as<From, void> && same_as<To, void>) ||
      requires { static_cast<rm_cvref<To>>(*((rm_cvref<From> *)nullptr)); };

  template <typename T, typename U>
  concept not_convertible_to =
      (!convertible_to<T, U>);

  template <typename T>
  concept strict_value_like =
      same_as<T, rm_ref<T>>;

  template <typename T>
  concept reference_like =
      same_as<T, rm_cvref<T> &>;

  template <typename T>
  concept const_reference_like =
      same_as<T, const rm_cvref<T> &>;

  template <typename T>
  concept double_reference_like =
      same_as<T, rm_cvref<T> &&>;

  template <typename T>
  concept const_double_reference_like =
      same_as<T, const rm_cvref<T> &&>;

  template <typename T>
  concept pointer_like =
      same_as<T, rm_pointer<T> *>;

  template <typename T>
  concept not_strict_value_like =
      (!strict_value_like<T>);

  template <typename T>
  concept not_reference_like =
      (!reference_like<T>);

  template <typename T>
  concept not_const_reference_like =
      (!const_reference_like<T>);

  template <typename T>
  concept not_double_reference_like =
      (!double_reference_like<T>);

  template <typename T>
  concept not_const_double_reference_like =
      (!const_double_reference_like<T>);

  template <typename T>
  concept not_pointer_like =
      (!pointer_like<T>);

  template <typename T, typename R, typename... A>
  concept callable = requires(T t, A &&...a) { 
    {t(a...)} -> convertible_to<R>; };

  template <typename P, typename... A>
  concept predicate = callable<P, bool, A...>;

  template <typename T, size_t N>
  concept size_more_than =
      (sizeof(T) > N);

  template <typename T, size_t N>
  concept size_lesseq_than =
      (sizeof(T) <= N);

  //-------------------------------------
  //
  // Meta : has operators concepts
  //
  //-------------------------------------

  template <typename T>
  concept notable =
      requires(T t) { !t; };

  template <typename T>
  concept prefix_incrementable =
      requires(T t) { ++t; };

  template <typename T>
  concept suffix_incrementable =
      requires(T t) { t++; };

  template <typename T>
  concept incrementable =
      prefix_incrementable<T> ||
      suffix_incrementable<T>;

  template <typename T>
  concept prefix_decrementable =
      requires(T t) { ++t; };

  template <typename T>
  concept suffix_decrementable =
      requires(T t) { t++; };

  template <typename T>
  concept decrementable =
      prefix_decrementable<T> ||
      suffix_decrementable<T>;

  template <typename T>
  concept indirectlyable =
      requires(T t) { *t; };

  template <typename T>
  concept addressable =
      requires(T t) { &t; };

  template <typename T, typename U>
  concept equality_comparable =
      requires(T t, U u) { t == u; };

  template <typename T, typename U>
  concept less_comparable =
      requires(T t, U u) { t < u; };

  template <typename T, typename U>
  concept greater_comparable =
      requires(T t, U u) { t > u; };

  template <typename T, typename U>
  concept lesseq_comparable =
      requires(T t, U u) { t <= u; };

  template <typename T, typename U>
  concept greatereq_comparable =
      requires(T t, U u) { t >= u; };

  template <typename T, typename U>
  concept and_comparable =
      requires(T t, U u) { t and u; };

  template <typename T, typename U>
  concept or_comparable =
      requires(T t, U u) { t or u; };

  template <typename T, typename U>
  concept addable =
      requires(T t, U u) { t + u; };

  template <typename T, typename U>
  concept substractable =
      requires(T t, U u) { t - u; };

  template <typename T, typename U>
  concept multiplyable =
      requires(T t, U u) { t *u; };

  template <typename T, typename U>
  concept dividable =
      requires(T t, U u) { t / u; };

  template <typename T, typename U>
  concept modulable =
      requires(T t, U u) { t % u; };

  //------------------------------------
  //
  // Meta : type familly
  //
  //------------------------------------

  template <typename T>
  concept character =
      same_one_of<T, char, wchar_t>;

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
  concept plain_integral =
      integral<T> || character<T>;

  template <typename T>
  concept floating_point =
      same_one_of<T, float, double, long double>;

  template <typename T>
  concept native_number =
      integral<T> ||
      character<T> ||
      floating_point<T> ||
      same_as<T, bool>;

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

  namespace impl
  {
    template <typename T>
    constexpr bool bounded_array_like = false;

    template <typename T, size_t N>
    constexpr bool bounded_array_like<T[N]> = true;

    template <typename T, size_t N>
    constexpr bool bounded_array_like<T (&)[N]> = true;
  }

  template <typename T>
  concept bounded_array_like = impl::bounded_array_like<T>;

  namespace impl
  {
    template <typename T>
    constexpr bool unbounded_array_like = false;

    template <typename T>
    constexpr bool unbounded_array_like<T[]> = true;

    template <typename T>
    constexpr bool unbounded_array_like<T (&)[]> = true;
  }

  template <typename T>
  concept unbounded_array_like = impl::unbounded_array_like<T>;

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

  template <not_const_reference_like T>
  constexpr T &&forward(rm_ref<T> &&t) noexcept
  {
    return static_cast<T &&>(t);
  }

  template <typename T>
  constexpr add_rref<rm_ref<T>> transfer(T &&t)
  {
    return static_cast<add_rref<rm_ref<T>>>(t);
  }

  template <native_number T>
  constexpr auto transfer(T &t)
  {
    T copy = static_cast<rm_ref<T> &&>(t);
    t = 0;
    return copy;
  }

  template <pointer_like T>
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
    auto get()
    {
      return _ptr;
    }

    auto get() const
    {
      return _ptr;
    }

  public:
    operator bool() const
    {
      return _ptr != nullptr;
    }

    auto operator*() -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return (*_ptr);
    }

    auto operator*() const
        -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return (*_ptr);
    }

    auto operator->() -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return _ptr;
    }

    auto operator->() const -> decltype(auto)
      requires(!native_array_like<T>)
    {
      return _ptr;
    }

    auto operator[](size_t i) -> decltype(auto)
      requires(native_array_like<T>)
    {
      return (_ptr[i]);
    }

    auto operator[](size_t i) const -> decltype(auto)
      requires(native_array_like<T>)
    {
      return (_ptr[i]);
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
    constexpr forward_reference(T t) : _t(transfer(t)) {}
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

    constexpr operator const T &() const
    {
      return _t;
    }

    constexpr const T &get() const
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
    constexpr operator T &&()
    {
      return _t.get();
    }

    constexpr T &&get()
    {
      return _t.get();
    }

    constexpr operator const T &&() const
    {
      return _t.get();
    }

    constexpr const T &&get() const
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
    constexpr operator T &()
    {
      return _t.get();
    }

    constexpr T &get()
    {
      return _t.get();
    }
    constexpr operator const T &() const
    {
      return _t.get();
    }

    constexpr const T &get() const
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
  // Maybe
  //
  //----------------------------------

  struct maybe_empty
  {
  };

  template <typename T>
  class maybe
  {
  private:
    bool _has = false;

    union
    {
      T _t;
      maybe_empty _e;
    };

  public:
    constexpr ~maybe()
    {
      if (_has)
      {
        _t.~T();
      }
    }

    constexpr maybe() : _has(false), _e() {}

    constexpr maybe(const T &t) : _has(true), _t(t) {}
    constexpr maybe(T &&t) : _has(true), _t(transfer(t)) {}

    constexpr maybe(const maybe &o) : _has(o._has)
    {
      if (_has)
      {
        _t = o._t;
      }
    }

    constexpr maybe(maybe &&o) : _has(transfer(o._has))
    {
      if (_has)
      {
        _t = transfer(o._t);
      }
    }

    constexpr maybe &operator=(const T &t)
    {
      _has = true;
      _t = t;
      return *this;
    }

    constexpr maybe &operator=(T &&t)
    {
      _has = true;
      _t = transfer(t);
      return *this;
    }

    constexpr maybe &operator=(maybe o)
    {
      _has = transfer(o._has);

      if (_has)
      {
        _t = transfer(o._t);
      }

      return *this;
    }

  public:
    constexpr operator T &()
    {
      return _t;
    }

    constexpr operator const T &() const
    {
      return _t;
    }

    constexpr T &operator*()
    {
      return _t;
    }

    constexpr const T &operator*() const
    {
      return _t;
    }

    constexpr bool has() const
    {
      return _has;
    }

  public:
    template <typename F>
    constexpr auto map(F &&f) &
    {
      using U = decltype(forward<F>(f)(static_cast<T &>(_t)));

      if (_has)
      {
        return maybe<U>(forward<F>(f)(static_cast<T &>(_t)));
      }
      else
      {
        return maybe<U>();
      }
    }

    template <typename F>
    constexpr auto map(F &&f) const &
    {
      using U = decltype(forward<F>(f)(static_cast<const T &>(_t)));

      if (_has)
      {
        return maybe<U>(forward<F>(f)(static_cast<const T &>(_t)));
      }
      else
      {
        return maybe<U>();
      }
    }

    template <typename F>
    constexpr auto map(F &&f) &&
    {

      using U = decltype(forward<F>(f)(static_cast<T &&>(_t)));

      if (_has)
      {
        return maybe<U>(forward<F>(f)(static_cast<T &&>(_t)));
      }
      else
      {
        return maybe<U>();
      }
    }

    template <typename F>
    constexpr auto map(F &&f) const &&
    {
      using U = decltype(forward<F>(f)(static_cast<const T &&>(_t)));

      if (_has)
      {
        return maybe<U>(forward<F>(f)(static_cast<const T &&>(_t)));
      }
      else
      {
        return maybe<U>();
      }
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
        : _handler(o._handler.get() == nullptr ? nullptr : o._handler->clone()),
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
    bool empty() const
    {
      return _handler.get() == nullptr && _func == nullptr;
    }

    template <typename... T>
    R operator()(T &&...t)
    {
      return _handler.get() != nullptr
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

  template <typename... T>
  tuple(T...) -> tuple<T...>;

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

  template <typename T0, typename T1>
  using pair = tuple<T0, T1>;

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
  concept forward_incrementable_iterator =
      requires(T t) { {t==t} -> convertible_to<bool>; } &&
      requires(T t) { {t!=t} -> convertible_to<bool>; } &&
      requires(T t) { {++t} -> same_one_of<T&, T>; } &&
      requires(T t) { {t++} -> same_one_of<T&, T>; };

  template <typename T>
  concept backward_incrementable_iterator =
      requires(T t) { {t==t} -> convertible_to<bool>; } &&
      requires(T t) { {t!=t} -> convertible_to<bool>; } &&
      requires(T t) { {--t} -> same_one_of<T&, T>; } &&
      requires(T t) { {t--} -> same_one_of<T&, T>; };

  template <typename T>
  concept input_iterator =
      forward_incrementable_iterator<T> &&
      (
          requires(T t) { {*t} -> const_double_reference_like; } ||
          requires(T t) { {*t} -> double_reference_like; } ||
          requires(T t) { {*t} -> reference_like; } ||
          requires(T t) { {*t} -> const_reference_like; } ||
          requires(T t) { {*t} -> strict_value_like; });

  template <typename T>
  concept output_iterator =
      forward_incrementable_iterator<T> &&
      requires(T t) { {*t} -> reference_like; };

  template <typename T>
  concept input_or_output_iterator =
      input_iterator<T> ||
      output_iterator<T>;

  template <typename T>
  concept equality_comparable_iterator =
      equality_comparable<T, T>;

  template <typename T>
  concept forward_iterator =
      input_or_output_iterator<T> &&
      equality_comparable_iterator<T>;

  template <typename T>
  concept backward_iterator =
      input_or_output_iterator<T> &&
      backward_incrementable_iterator<T> &&
      equality_comparable_iterator<T>;

  template <typename T>
  concept bidirectional_iterator =
      forward_iterator<T> &&
      backward_iterator<T>;

  template <typename T>
  concept random_iterator =
      bidirectional_iterator<T> &&
      requires(T t) { { t[0] }; };

  template <typename T>
  concept distanciable_iterator =
      substractable<T, T>;

  template <typename C>
  concept input_range =
      requires(C c) {{c.begin()} -> input_iterator; } &&
      requires(C c) {{c.end()} -> input_iterator; };

  template <typename C>
  concept output_range =
      requires(C c) {{c.begin()} -> output_iterator; } &&
      requires(C c) {{c.end()} -> output_iterator; };

  template <typename C>
  concept range =
      requires(C c) {{c.begin()} -> input_or_output_iterator; } &&
      requires(C c) {{c.end()} -> input_or_output_iterator; };

  template <typename C>
  concept forward_range =
      requires(C c) {{c.begin()} -> forward_iterator; } &&
      requires(C c) {{c.end()} -> forward_iterator; };

  template <typename C>
  concept backward_range =
      requires(C c) {{c.begin()} -> backward_iterator; } &&
      requires(C c) {{c.end()} -> backward_iterator; };

  template <typename C>
  concept bidirectional_range =
      requires(C c) {{c.begin()} -> bidirectional_iterator; } &&
      requires(C c) {{c.end()} -> bidirectional_iterator; };

  template <typename C>
  concept random_range =
      requires(C c) {{c.begin()} -> random_iterator; } &&
      requires(C c) {{c.end()} -> random_iterator; };

  template <typename C>
  concept range_or_bounded_array =
      range<C> || bounded_array_like<C>;

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

  template <typename C, typename T>
  concept push_container =
      requires(C c, T t) { c.push(t); };

  template <typename C, typename T>
  concept pop_container =
      requires(C c) { { c.pop() } -> convertible_to<T&>; } ||
      requires(C c) { { c.pop() } -> convertible_to<T>; } ||
      requires(C c) { { c.pop() } -> convertible_to<const T&>; } ||
      requires(C c) { { c.pop() } -> convertible_to<T&&>; } ||
      requires(C c) { { c.pop() } -> convertible_to<const T&&>; };

  template <typename C, typename T>
  concept container =
      push_container<C, T> &&
      pop_container<C, T>;

  template <range R>
  constexpr auto begin(R &&r)
  {
    return forward<R>(r).begin();
  }

  template <range R>
  constexpr auto end(R &&r)
  {
    return forward<R>(r).end();
  }

  template <typename T, size_t N>
  constexpr auto begin(T (&ar)[N])
  {
    return ar;
  }

  template <typename T, size_t N>
  constexpr auto end(T (&ar)[N])
  {
    return ar + N;
  }

  template <input_or_output_iterator I>
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

    template <range_or_bounded_array R>
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
      requires distanciable_iterator<I>
    {
      return _begin == nullptr ? 0 : _end - _begin;
    }

    constexpr auto empty() const
      requires distanciable_iterator<I>
    {
      return size() == 0;
    }

    constexpr const auto &operator[](size_t i) const
      requires random_iterator<I>
    {
      return (_begin[i]);
    }
  };

  template <range R>
  view(R &&r) -> view<decltype(begin(forward<R>(r)))>;

  template <input_or_output_iterator I>
  view(I b, I e) -> view<I>;

  template <input_range R1, input_range R2>
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

  template <input_range R1, input_range R2>
  constexpr bool operator==(const R1 &r1, const R2 &r2)
  {
    return stew::equals(r1, r2);
  }

  template <input_range R1, input_range R2>
  constexpr bool operator!=(const R1 &r1, const R2 &r2)
  {
    return !stew::equals(r1, r2);
  }

  template <input_range R1, input_range R2>
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

  template <input_range R, typename T>
  constexpr bool starts_with(const R &r, const T &t)
  {
    auto b = begin(r);
    return b != end(r) && *b == t;
  }

  template <input_range R, typename T>
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

  template <input_range R1, range R2>
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

  template <input_range R, predicate<range_const_reference<R>> P>
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

  template <input_range R, equality_comparable<range_value_type<R>> T>
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

  template <input_range R, predicate<range_const_reference<R>> P>
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

  template <input_range R, typename T>
  constexpr bool contains(const R &r, const T &t)
  {
    return find(r, t) != end(r);
  }

  template <input_range R1, input_range R2>
  constexpr bool contains(const R1 &r1, const R2 &r2)
  {
    return find(r1, r2) != end(r1);
  }

  template <input_range R1, predicate<range_const_reference<R1>> P>
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

  template <input_range R1, predicate<range_const_reference<R1>> P>
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

  template <input_range R1, predicate<range_const_reference<R1>> P>
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

  template <input_range R, output_iterator I>
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

  template <typename G>
  class generator_iterator
  {
    G _gener;
    decltype(_gener()) _value;

  public:
    constexpr generator_iterator(G &&gener)
        : _gener(transfer(gener)), _value(_gener()) {}

    template <convertible_to<decltype(_value)> T>
    constexpr generator_iterator(G &&gener, T &&sentinel)
        : _gener(transfer(gener)),
          _value(forward<T>(sentinel)) {}

  public:
    constexpr auto operator==(const generator_iterator &o) const
    {
      return _value == o._value;
    }

    constexpr generator_iterator &operator++()
    {
      _value = _gener();
      return *this;
    }

    constexpr generator_iterator operator++(int)
    {
      auto copy = *this;
      ++*this;
      return copy;
    }

    constexpr auto operator*() -> decltype(auto)
    {
      return _value;
    }

    constexpr auto operator-(const generator_iterator &o) const
    {
      return _value - o._value;
    }
  };

  namespace impl
  {
    template <plain_integral I>
    class incrementer
    {
    private:
      I _current = 0;
      I _step = 1;

    public:
      constexpr incrementer(I current, I step)
          : _current(current), _step(step) {}

    public:
      constexpr I operator()()
      {
        auto copy = _current;
        _current += _step;
        return copy;
      }
    };
  }

  template <plain_integral I>
  constexpr auto upto(I from, I to, I step = 1)
  {
    assert(from <= to);
    assert(((to - from) % step) == 0);

    return view<generator_iterator<impl::incrementer<I>>>(
        generator_iterator<impl::incrementer<I>>(impl::incrementer<I>(from, step)),
        generator_iterator<impl::incrementer<I>>(impl::incrementer<I>(to, step), to));
  }

  template <plain_integral I>
  constexpr auto downto(I from, I to, I step = 1)
  {
    assert(from >= to);
    assert(((from - to) % step) == 0);

    return view<generator_iterator<impl::incrementer<I>>>(
        generator_iterator<impl::incrementer<I>>(impl::incrementer<I>(from, -step)),
        generator_iterator<impl::incrementer<I>>(impl::incrementer<I>(to, -step), to));
  }

  template <typename T, push_container<T> C>
  class push_iterator
  {
  private:
    C *_container = nullptr;

  public:
    constexpr push_iterator(C &container)
        : _container(&container)
    {
    }

    push_iterator &operator=(const T &t)
    {
      _container->push(t);
      return *this;
    }

    push_iterator &operator=(T &&t)
    {
      _container->push(transfer(t));
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

    auto operator==(const push_iterator &o) const
    {
      return _container == o._container;
    }
  };

  template <typename T, push_container<T> C>
  push_iterator<T, C> push_inserter(C &c)
  {
    return push_iterator<T, C>(c);
  }

  template <input_iterator I>
  class transfer_iterator
  {
  private:
    I _iter;

  public:
    constexpr transfer_iterator(I iter)
        : _iter(iter) {}

  public:
    constexpr auto operator*() -> decltype(auto)
    {
      return transfer(*_iter);
    }

    constexpr bool operator==(const transfer_iterator &o) const
    {
      return _iter == o._iter;
    }

    constexpr transfer_iterator &operator++()
    {
      ++_iter;
      return *this;
    }

    constexpr transfer_iterator operator++(int)
    {
      auto copy = *this;
      ++*this;
      return copy;
    }

    constexpr auto operator-(const transfer_iterator &o) const
      requires distanciable_iterator<I>
    {
      return _iter - o._iter;
    }
  };

  template <input_iterator I>
  constexpr view<transfer_iterator<I>> transfer_view(I b, I e)
  {
    return view<transfer_iterator<I>>(
        transfer_iterator<I>(b),
        transfer_iterator<I>(e));
  }

  template <input_range R>
  constexpr auto transfer_view(R &&r)
  {
    return transfer_view(
        begin(forward<R>(r)),
        end(forward<R>(r)));
  }

  template <bidirectional_iterator I>
  class reverse_iterator
  {
  protected:
    I _iter;

  public:
    constexpr reverse_iterator(I iter) : _iter(iter) {}

    constexpr bool operator==(const reverse_iterator &o) const
    {
      return _iter == o._iter;
    }

    constexpr decltype(auto) operator*() const
    {
      auto copy = _iter;
      --copy;
      return *copy;
    }

    constexpr reverse_iterator &operator++()
    {
      --_iter;
      return *this;
    }
    constexpr reverse_iterator operator++(int)
    {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }

    constexpr reverse_iterator &operator--()
    {
      ++_iter;
      return *this;
    }

    constexpr reverse_iterator operator--(int)
    {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    constexpr auto operator-(const reverse_iterator &o) const
      requires distanciable_iterator<I>
    {
      return _iter - o._iter;
    }
  };

  template <bidirectional_iterator I>
  constexpr view<reverse_iterator<I>> reverse_view(I b, I e)
  {
    return view<reverse_iterator<I>>(
        reverse_iterator<I>(e),
        reverse_iterator<I>(b));
  }

  template <bidirectional_range R>
  constexpr auto reverse_view(R &&r)
  {
    return reverse_view(
        begin(forward<R>(r)),
        end(forward<R>(r)));
  }

  template <input_iterator I, typename M>
  class map_iterator
  {
  private:
    I _iter;
    M _map;

  public:
    constexpr map_iterator(I iter, M map)
        : _iter(transfer(iter)),
          _map(transfer(map))
    {
    }

  public:
    constexpr map_iterator &operator++()
    {
      ++_iter;
      return *this;
    }

    constexpr map_iterator operator++(int)
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr bool operator==(const map_iterator &o) const
    {
      return _iter == o._iter;
    }

    constexpr auto operator*() const
    {
      return _map(*_iter);
    }

    constexpr auto operator-(const map_iterator &o) const
      requires distanciable_iterator<I>
    {
      return _iter - o._iter;
    }
  };

  template <input_iterator I, typename M>
  constexpr view<map_iterator<I, M>> map_view(I b, I e, M map)
  {
    return view<map_iterator<I, M>>(
        map_iterator<I, M>(b, map),
        map_iterator<I, M>(e, map));
  }

  template <input_range R, typename M>
  constexpr auto map_view(R &&r, M map)
  {
    return map_view(
        begin(forward<R>(r)),
        end(forward<R>(r)), map);
  }

  template <input_iterator I, predicate<decltype(*I{})> F>
  class filter_iterator
  {
  private:
    I _iter;
    I _end;
    F _filter;

  public:
    constexpr filter_iterator(I iter, I end, F filter)
        : _iter(iter), _end(end), _filter(filter) {}

  public:
    constexpr filter_iterator &operator++()
    {
      ++_iter;

      while (_iter != _end && !_filter(*_iter))
      {
        ++_iter;
      }

      return *this;
    }

    constexpr filter_iterator operator++(int)
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    constexpr bool operator==(const filter_iterator &o) const
    {
      return _iter == o._iter;
    }

    constexpr auto operator*()
    {
      while (_iter != _end && !_filter(*_iter))
      {
        ++_iter;
      }

      return *_iter;
    }

    constexpr auto operator-(const filter_iterator &o) const
      requires distanciable_iterator<I>
    {
      return _iter - o._iter;
    }
  };

  template <input_iterator I, typename F>
  constexpr view<filter_iterator<I, F>> filter_view(I b, I e, F filter)
  {
    return view<filter_iterator<I, F>>(
        filter_iterator<I, F>(b, e, filter),
        filter_iterator<I, F>(e, e, filter));
  }

  template <input_range R, typename F>
  constexpr auto filter_view(R &&r, F filter)
  {
    return filter_view(
        begin(forward<R>(r)),
        end(forward<R>(r)), filter);
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

    constexpr const auto begin() const
    {
      return _data;
    }

    constexpr const auto end() const
    {
      return _data + N;
    }

    constexpr T &operator[](size_t i)
    {
      assert(i < N);
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      assert(i < N);
      return _data[i];
    }
  };

  template <size_t I, typename T, size_t N>
  constexpr auto get(array<T, N> &a) -> decltype(auto)
  {
    static_assert(I < N);
    return a[I];
  }

  template <size_t I, typename T, size_t N>
  constexpr auto get(const array<T, N> &a) -> decltype(auto)
  {
    static_assert(I < N);
    return a[I];
  }

  template <size_t I, typename T, size_t N>
  constexpr auto get(array<T, N> &&a) -> decltype(auto)
  {
    static_assert(I < N);
    return a[I];
  }

  template <size_t I, typename T, size_t N>
  constexpr auto get(const array<T, N> &&a) -> decltype(auto)
  {
    static_assert(I < N);
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

    template <input_range R>
    constexpr static_stack(R &&r)
    {
      push(forward<R>(r));
    }

    constexpr static_stack(const static_stack &) = default;
    constexpr static_stack(static_stack &&) = default;
    constexpr static_stack &operator=(const static_stack &) = default;
    constexpr static_stack &operator=(static_stack &&) = default;

    template <input_range R>
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
      return stew::begin(_data) + _idx;
    }

    constexpr auto end()
    {
      return stew::end(_data);
    }

    constexpr const auto begin() const
    {
      return stew::begin(_data) + _idx;
    }

    constexpr const auto end() const
    {
      return stew::end(_data);
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

    template <input_range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter<T>(*this));
    }

    constexpr maybe<T> pop()
    {
      if (0 <= _idx && _idx < N)
      {
        return maybe<T>(transfer(_data[_idx++]));
      }
      else
      {
        return maybe<T>();
      }
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

    template <input_range R>
    constexpr fixed_stack(R &&r)
      requires distanciable_iterator<decltype(stew::begin(r))>
        : fixed_stack(stew::end(r) - stew::begin(r))
    {
      push(forward<R>(r));
    }

    constexpr fixed_stack(const fixed_stack &) = default;
    constexpr fixed_stack(fixed_stack &&) = default;
    constexpr fixed_stack &operator=(const fixed_stack &) = default;
    constexpr fixed_stack &operator=(fixed_stack &&) = default;

    template <input_range R>
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

    constexpr const auto begin() const
    {
      return _data.get() + _idx;
    }

    constexpr const auto end() const
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

    template <input_range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter<T>(*this));
    }

    constexpr maybe<T> pop()
    {
      if (0 <= _idx && _idx < _max)
      {
        return maybe<T>(transfer(_data[_idx++]));
      }
      else
      {
        return maybe<T>();
      }
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
    template <input_range R>
    constexpr stack(R &&r)
      requires distanciable_iterator<decltype(stew::begin(r))>
        : stack(stew::end(r) - stew::begin(r))
    {
      push(forward<R>(r));
    }

    constexpr stack(const stack &) = default;
    constexpr stack(stack &&) = default;

    constexpr stack &operator=(const stack &) = default;
    constexpr stack &operator=(stack &&) = default;

    template <input_range R>
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

    constexpr const auto begin() const
    {
      return _data.begin();
    }

    constexpr const auto end() const
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

    template <input_range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter<T>(*this));
    }

    constexpr auto pop()
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

    template <input_range R>
    constexpr static_vector(R &&r)
    {
      push(forward<R>(r));
    }

    constexpr static_vector(const static_vector &) = default;
    constexpr static_vector(static_vector &&) = default;
    constexpr static_vector &operator=(const static_vector &) = default;
    constexpr static_vector &operator=(static_vector &&) = default;

    template <input_range R>
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

    constexpr const auto begin() const
    {
      return _data.begin();
    }

    constexpr const auto end() const
    {
      return _data.begin() + _size;
    }

    constexpr T &operator[](size_t i)
    {
      assert(i < N);
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      assert(i < N);
      return _data[i];
    }

  public:
    template <convertible_to<T> U>
    constexpr void push(U &&u)
    {
      if (!full())
      {
        _data[_size] = forward<U>(u);
        ++_size;
      }
    }

    template <input_range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter<T>(*this));
    }

    constexpr maybe<T> pop()
    {
      if (_size != 0)
      {
        return maybe<T>(transfer(_data[_size--]));
      }
      else
      {
        return maybe<T>();
      }
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
      push(o);
    }

    constexpr fixed_vector(fixed_vector &&o)
        : _size{transfer(o._size)},
          _max{transfer(o._max)},
          _data{transfer(o._data)}
    {
    }

    template <input_range R>
    constexpr fixed_vector(R &&r)
      requires distanciable_iterator<decltype(stew::begin(r))>
        : fixed_vector(stew::end(forward<R>(r)) -
                       stew::begin(forward<R>(r)))
    {
      push(forward<R>(r));
    }

    constexpr fixed_vector &operator=(fixed_vector o)
    {
      _size = transfer(o._size);
      _max = transfer(o._max);
      _data = transfer(o._data);
      return *this;
    }

    template <input_range R>
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

    constexpr const auto begin() const
    {
      return _data.get();
    }

    constexpr const auto end() const
    {
      return begin() + _size;
    }

    constexpr T &operator[](size_t i)
    {
      assert(i < _size);
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      assert(i < _size);
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
    constexpr void push(U &&u)
    {
      if (!full())
      {
        _data[_size] = forward<U>(u);
        _size += 1;
      }
    }

    template <input_range R>
    constexpr void push(R &&r)
      requires not_convertible_to<R, T>
    {
      copy(forward<R>(r), push_inserter<T>(*this));
    }

    constexpr maybe<T> pop()
    {
      if (_size != 0)
      {
        return maybe<T>(transfer(_data[_size--]));
      }
      else
      {
        return maybe<T>();
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

    template <input_range R>
    constexpr vector(R &&r)
        : vector(stew::end(forward<R>(r)) -
                 stew::begin(forward<R>(r)))
    {
      push(forward<R>(r));
    }

    constexpr vector(const vector &) = default;
    constexpr vector(vector &) = default;
    constexpr vector &operator=(const vector &) = default;
    constexpr vector &operator=(vector &&) = default;

    template <input_range R>
    constexpr vector &operator=(R &&r)
    {
      return (*this = transfer(vector(forward<R>(r))));
    }

  public:
    constexpr auto begin()
    {
      return stew::begin(_data);
    }

    constexpr auto end()
    {
      return stew::end(_data);
    }

    constexpr const auto begin() const
    {
      return stew::begin(_data);
    }

    constexpr const auto end() const
    {
      return stew::end(_data);
    }

    constexpr T &operator[](size_t i)
    {
      assert(i < size());
      return _data[i];
    }

    constexpr const T &operator[](size_t i) const
    {
      assert(i < size());
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
    constexpr void push(U &&u)
    {
      if (_data.full())
      {
        fixed_vector<T> tmp = transfer(_data);
        _data = fixed_vector<T>(tmp.size() * 2 + 10);
        _data.push(transfer(tmp));
      }

      _data.push(forward<U>(u));
    }

    template <input_range R>
    constexpr void push(R &&r)
    {
      copy(forward<R>(r), push_inserter<T>(*this));
    }

    constexpr auto pop()
    {
      return _data.pop();
    }
  };

  //----------------------------
  //
  // String classes
  //
  //----------------------------

  template <character C>
  using string_view = view<const C *>;

  template <character C>
  constexpr string_view<C> substr(string_view<C> s, size_t from)
  {
    return string_view<C>(begin(s) + from, end(s));
  }

  template <character C>
  constexpr string_view<C> substr(string_view<C> s, size_t from, size_t n)
  {
    return string_view<C>(begin(s) + from, begin(s) + from + n);
  }

  constexpr string_view<char> operator"" _sv(const char *s, size_t n)
  {
    return string_view<char>(s, s + n);
  }

  constexpr string_view<wchar_t> operator"" _sv(const wchar_t *s, size_t n)
  {
    return string_view<wchar_t>(s, s + n);
  }

  template <typename S, typename C>
  concept string_view_like =
      character<C> && convertible_to<S, string_view<C>>;

  template <character C, size_t N>
  using static_string = static_vector<C, N>;

  template <character C>
  using fixed_string = fixed_vector<C>;

  template <character C>
  using string = vector<C>;

  string<char> operator"" _s(const char *s, size_t n)
  {
    return string<char>(string_view<char>(s, s + n));
  }

  string<wchar_t> operator"" _s(const wchar_t *s, size_t n)
  {
    return string<wchar_t>(string_view<wchar_t>(s, s + n));
  }

  template <character C>
  struct after_pair
  {
    bool _found;
    string_view<C> _aft;
  };

  template <character C>
  constexpr after_pair<C> after(
      string_view<C> input,
      string_view<C> sep)
  {
    auto pos = find(input, sep);

    return after_pair<C>{
        pos != end(input),
        {pos + sep.size(), end(input)}};
  }

  template <character C>
  struct around_pair
  {
    bool _found;
    string_view<C> _bef;
    string_view<C> _aft;
  };

  template <character C>
  constexpr around_pair<C> around(
      string_view<C> input,
      string_view<C> sep)
  {
    auto pos = find(input, sep);

    if (pos != end(input))
    {
      return around_pair<C>{
          true,
          {begin(input), pos},
          {pos + sep.size(), end(input)}};
    }
    else
    {
      return around_pair<C>{
          false,
          {begin(input), begin(input)},
          {begin(input), end(input)}};
    }
  }

  template <character C>
  constexpr around_pair<C> around(
      string_view<C> input,
      C sep)
  {
    auto pos = find(input, sep);

    if (pos != end(input))
    {
      return around_pair<C>{
          true,
          {begin(input), pos},
          {pos + 1, end(input)}};
    }
    else
    {
      return around_pair<C>{
          false,
          {begin(input), begin(input)},
          {begin(input), end(input)}};
    }
  }

  //---------------------
  //
  // I/O Containers
  //
  //---------------------

  constexpr array<const char *, 3> modechr = {"r", "w"};

  enum class mode : size_t
  {
    r = 0,
    w = 1
  };

  template <typename T, mode m>
  class file
  {
  private:
    FILE *_fp = nullptr;

  public:
    ~file()
    {
      close();
    }

    file() = default;

    template <character C>
    file(string_view<C> path)
        : _fp(fopen(path.begin(), modechr[size_t(m)]))
    {
    }

    file(FILE *fp) : _fp(fp) {}

    file(const file &) = delete;
    file(file &&) = default;
    file &operator=(const file &) = delete;
    file &operator=(file &&) = default;

  public:
    class ofile_iterator
    {
    private:
      non_owning<file<T, m>> _file = nullptr;

    public:
      ~ofile_iterator() = default;
      ofile_iterator() = default;
      ofile_iterator(file &f) : _file(&f) {}
      ofile_iterator(const ofile_iterator &) = default;
      ofile_iterator(ofile_iterator &&) = default;
      ofile_iterator &operator=(const ofile_iterator &) = default;
      ofile_iterator &operator=(ofile_iterator &&) = default;

    public:
      ofile_iterator &operator*()
      {
        return *this;
      }

      ofile_iterator &operator++()
      {
        return *this;
      }

      ofile_iterator &operator++(int)
      {
        return *this;
      }

      ofile_iterator &operator=(const T &t)
      {
        if (_file.get() != nullptr)
        {
          _file.get()->push(t);
        }
      }

      ofile_iterator &operator=(T &&t)
      {
        if (_file.get() != nullptr)
        {
          _file.get()->push(t);
        }
      }
    };

    class ifile_iterator
    {
    private:
      non_owning<file<T, m>> _file = nullptr;
      maybe<T> _value;

    public:
      ~ifile_iterator() = default;
      ifile_iterator() = default;
      ifile_iterator(file &f) : _file(&f)
      {
        ++(*this);
      }

      ifile_iterator(const ifile_iterator &) = default;
      ifile_iterator(ifile_iterator &&) = default;
      ifile_iterator &operator=(const ifile_iterator &) = default;
      ifile_iterator &operator=(ifile_iterator &&) = default;

    public:
      auto operator==(const ifile_iterator &o) const
      {
        return _file.get() == o._file.get();
      }

      maybe<T> &operator*()
      {
        return _value;
      }

      ifile_iterator &operator++()
      {
        if (_file.get() != nullptr)
        {
          _value = _file.get()->pop();

          if (!_value.has())
          {
            _file = nullptr;
          }
        }

        return *this;
      }

      ifile_iterator &operator++(int)
      {
        return ++(*this);
      }
    };

  public:
    bool opened()
    {
      return _fp != nullptr;
    }

  public:
    auto begin()
    {
      if constexpr (m == mode::w)
      {
        return ofile_iterator(*this);
      }
      else if constexpr (m == mode::r)
      {
        return ifile_iterator(*this);
      }
    }

    auto end()
    {
      if constexpr (m == mode::w)
      {
        return ofile_iterator();
      }
      else if constexpr (m == mode::r)
      {
        return ifile_iterator();
      }
    }

    auto begin() const
      requires(m == mode::r)
    {
      if constexpr (m == mode::r)
      {
        return ifile_iterator(*this);
      }
    }

    auto end() const
      requires(m == mode::r)
    {
      if constexpr (m == mode::r)
      {
        return ifile_iterator();
      }
    }

  public:
    void push(T &&t)
      requires(m == mode::w)
    {
      if (_fp != nullptr)
      {
        fwrite(&t, sizeof(rm_cvref<T>), 1, _fp);
      }
    }

    void push(const T &t)
      requires(m == mode::w)
    {
      if (_fp != nullptr)
      {
        fwrite(&t, sizeof(rm_cvref<T>), 1, _fp);
      }
    }

    template <input_range R>
    void push(R &&r)
      requires(m == mode::w)
    {
      if constexpr (string_view_like<R, char>)
      {
        if (_fp != nullptr)
        {
          string_view<char> tmp(forward<R>(r));
          fwrite(tmp.begin(), sizeof(char), tmp.size(), _fp);
        }
      }
      else if constexpr (string_view_like<R, wchar_t>)
      {
        if (_fp != nullptr)
        {
          string_view<wchar_t> tmp(forward<R>(r));
          fwrite(tmp.begin(), sizeof(wchar_t), tmp.size(), _fp);
        }
      }
      else
      {
        for (auto &&i : forward<R>(r))
        {
          push(forward<decltype(i)>(i));
        }
      }
    }

    maybe<T> pop()
      requires(m == mode::r)
    {
      maybe<T> res;

      if (_fp != nullptr)
      {
        T buff;

        if (fread(&buff, sizeof(T), 1, _fp) == 1)
        {
          res = transfer(buff);
        }
      }

      return res;
    }

    template <push_container<T> R>
    void pop(R &r, size_t n)
      requires(m == mode::r)
    {
      if (_fp != nullptr)
      {
        T buff;

        for (size_t i : upto(size_t(0), n))
        {
          if (fread(&buff, sizeof(T), 1, _fp) == 1)
          {
            r.push(transfer(buff));
          }
          else
          {
            break;
          }
        }
      }
    }

  private:
    void close()
    {
      if (_fp != nullptr)
      {
        fclose(_fp);
        _fp = nullptr;
      }
    }
  };

  file<char, mode::r> termin(stdin);
  file<char, mode::w> termout(stdout);

  file<wchar_t, mode::r> wtermin(stdin);
  file<wchar_t, mode::w> wtermout(stdout);

  //------------------------------
  //
  // Formatting
  //
  //------------------------------

  template <typename O>
  concept ostream =
      (push_container<O, char> &&
       push_container<O, string_view<char>>) ||
      (push_container<O, wchar_t> &&
       push_container<O, string_view<wchar_t>>);

  template <character C, size_t N>
  class format_string
  {
  private:
    array<string_view<C>, N> _parts;

  public:
    template <string_view_like<C> FMT>
    consteval format_string(FMT &&fmt)
        : _parts(split(forward<FMT>(fmt)))
    {
    }

    constexpr const auto &parts() const
    {
      return _parts;
    }

  private:
    consteval auto split(string_view<C> fmt) const
    {
      array<string_view<C>, N> parts;

      for (auto &part : parts)
      {
        auto [fnd, bef, aft] =
            around(fmt, "{}"_sv);

        if (fnd)
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

  template <typename T>
  class formatter;

  template <ostream O, typename T>
  constexpr void format_to(O &o, const T &t)
  {
    formatter<T>::to(o, t);
  }

  namespace impl
  {
    template <size_t I, size_t N,
              ostream O, character C,
              typename H, typename... T>
    constexpr void format_to_one(
        O &o, const format_string<C, N> &fmt, const H &h, const T &...t)
    {
      format_to(o, get<I>(fmt.parts()));
      format_to(o, h);

      if constexpr (sizeof...(T) > 0)
      {
        format_to_one<I + 1>(o, fmt, t...);
      }
    }

    template <ostream O, character C, typename H, typename... T>
    constexpr void format_to(
        O &o, const format_string<C, sizeof...(T) + 2> &fmt,
        const H &h, const T &...t)
    {
      format_to_one<0>(o, fmt, h, t...);
      format_to(o, get<sizeof...(T) + 1>(fmt.parts()));
    }

    template <ostream O, character C>
    constexpr void format_to(O &o, const format_string<C, 1> &fmt)
    {
      format_to(o, get<0>(fmt.parts()));
    }
  }

  template <ostream O, typename... A>
  constexpr void format_to(O &o, const format_string<char, sizeof...(A) + 1> &fmt, const A &...a)
  {
    impl::format_to(o, fmt, a...);
  }

  template <ostream O, typename... A>
  constexpr void format_to(O &o, const format_string<wchar_t, sizeof...(A) + 1> &fmt, const A &...a)
  {
    impl::format_to(o, fmt, a...);
  }

  template <character C>
  class formatter<C>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, C o)
    {
      os.push(o);
    }
  };

  template <character C>
  class formatter<string_view<C>>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, string_view<C> o)
    {
      os.push(o);
    }
  };

  template <character C, size_t N>
  class formatter<const C (&)[N]>
      : public formatter<string_view<C>>
  {
  };

  template <character C, size_t N>
  class formatter<C (&)[N]>
      : public formatter<string_view<C>>
  {
  };

  template <character C, size_t N>
  class formatter<const C[N]>
      : public formatter<string_view<C>>
  {
  };

  template <character C, size_t N>
  class formatter<C[N]>
      : public formatter<string_view<C>>
  {
  };

  template <character C>
  class formatter<fixed_string<C>>
      : public formatter<string_view<C>>
  {
  };

  template <character C>
  class formatter<string<C>>
      : public formatter<string_view<C>>
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
        format_to(o, '0');
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
        format_to(o, '-');
      }

      format_to(o, tbuff);
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
        format_to(o, '0');
      }
      else
      {
        while (tmp != 0)
        {
          tbuff.push("0123456789"[tmp % 10]);
          tmp /= 10;
        }
      }

      format_to(o, tbuff);
    }
  };

  template <floating_point F>
  class formatter<F>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, F d)
    {
      size_t i = static_cast<size_t>(d);
      size_t e = static_cast<size_t>((d - i) * 10'000.0);
      format_to(o, i);
      format_to(o, '.');
      format_to(o, e);
    }
  };

  template <>
  class formatter<bool>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, bool b)
    {
      format_to(o, b ? "true"_sv : "false"_sv);
    }
  };

  template <pointer_like P>
  class formatter<P>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, P p)
    {
      format_to(os, (size_t)(void *)(p));
    }
  };

  //------------------------
  //
  // Extracting
  //
  //------------------------

  template <typename T>
  class extractor;

  template <typename... T>
  using extract_response = tuple<maybe<T>...>;

  namespace impl
  {
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

    template <size_t I, character C, typename T>
    constexpr void extract_to_one(
        string_view<C> &input,
        string_view<C> fmt_part,
        maybe<T> &response_part)
    {
      if (fmt_part.empty() || starts_with(input, fmt_part))
      {
        input = substr(input, fmt_part.size());
        input = extractor<T>::to(input, response_part);
      }
    }

    template <character C, typename... T, size_t... I>
    constexpr void extract_to(
        string_view<C> input,
        const array<string_view<C>, sizeof...(T) + 1> fmt_parts,
        extract_response<T...> &response,
        isequence<I...>)
    {
      (extract_to_one<I>(input, get<I>(fmt_parts), get<I>(response)), ...);
    }
  }

  template <typename... T>
  constexpr void extract_to(
      string_view<char> input,
      format_string<char, sizeof...(T) + 1> fmt,
      extract_response<T...> &response)
  {
    return impl::extract_to(
        input, fmt.parts(), response,
        impl::make_isequence<sizeof...(T)>());
  }

  template <typename... T>
  constexpr void extract_to(
      string_view<wchar_t> input,
      format_string<wchar_t, sizeof...(T) + 1> fmt,
      extract_response<T...> &response)
  {
    return impl::extract_to(
        input, fmt.parts(), response,
        impl::make_isequence<sizeof...(T)>());
  }

  template <unsigned_integral I>
  class extractor<I>
  {
  public:
    template <character C>
    constexpr static auto to(
        string_view<C> i, maybe<I> &mb)
    {
      auto b = begin(i);
      auto e = end(i);

      I res = 0;

      while (b != e && '0' <= *b && *b <= '9')
      {
        res = res * 10 + (*b - '0');
        ++b;
      }

      if (b != begin(i))
      {
        mb = res;
      }

      return string_view<C>(b, end(i));
    }
  };

  template <signed_integral I>
  class extractor<I>
  {
  public:
    template <character C>
    constexpr static auto to(
        string_view<C> i, maybe<I> &mb)
    {
      auto b = begin(i);
      auto e = end(i);

      I unit = 1;

      if (b != e)
      {
        if (*b == '-')
        {
          unit = -1;
          ++b;
        }
        else if (*b == '+')
        {
          ++b;
        }
      }

      I res = 0;

      while (b != e && '0' <= *b && *b <= '9')
      {
        res = res * 10 + (*b - '0');
        ++b;
      }

      if (b != begin(i))
      {
        mb = unit * res;
      }

      return string_view<C>(b, end(i));
    }
  };

  template <>
  class extractor<bool>
  {
  public:
    template <character C>
    constexpr static auto to(
        string_view<C> i, maybe<bool> &mb)
    {
      if (starts_with(i, "true"_sv))
      {
        mb = true;
        return substr(i, 4);
      }
      else if (starts_with(i, "false"_sv))
      {
        mb = false;
        return substr(i, 5);
      }
      else
      {
        return i;
      }
    }
  };

  //------------------------
  //
  // Console
  //
  //------------------------

  template <character C>
  class console
  {
  public:
    template <typename... T>
    static void printf(format_string<C, sizeof...(T) + 1> fmt, const T &...t)
    {
      format_to(termout, fmt, t...);
    }

    template <typename... T>
    static void printfln(format_string<C, sizeof...(T) + 1> fmt, const T &...t)
    {
      format_to(termout, fmt, t...);
      termout.push('\n');
    }

    static void print(string_view<C> s)
    {
      termout.push(s);
    }

    static void println(string_view<C> s)
    {
      termout.push(s);
      termout.push('\n');
    }

    template <typename... T>
    static void readf(
        format_string<C, sizeof...(T) + 1> fmt,
        extract_response<T...> &response)
    {
      maybe<C> mb;
      string<C> s;

      while ((mb = termin.pop()).has() && *mb != '\n')
      {
        s.push(*mb);
      }

      extract_to(s, fmt, response);
    }
  };

  //------------------------
  //
  //  TImers and Calendars
  //
  //------------------------

  namespace time
  {
    enum class unit
    {
      ns,
      us,
      ms,
      s
    };
  }

  template <time::unit u>
  class cpu_timer
  {
  private:
    clock_t _t0;

  public:
    inline cpu_timer() : _t0(::clock()) {}

  public:
    inline double duration() const
    {
      const double clocks = (1.0 * (clock() - _t0));
      const double dur = clocks / CLOCKS_PER_SEC;

      if constexpr (u == time::unit::s)
      {
        return dur;
      }
      else if constexpr (u == time::unit::ms)
      {
        return 1'000.0 * dur;
      }
      else if constexpr (u == time::unit::us)
      {
        return 1'000'000.0 * dur;
      }
      else if constexpr (u == time::unit::ns)
      {
        return 1'000'000'000.0 * dur;
      }
    }
  };

  template <time::unit u>
  class formatter<cpu_timer<u>>
  {
  public:
    template <ostream O>
    static constexpr void to(O &o, const cpu_timer<u> &timer)
    {
      format_to(o, timer.duration());
    }
  };

  class wall_timer
  {
  private:
    time_t _t0;

  public:
    inline wall_timer() : _t0(::time(nullptr)) {}

  public:
    inline double duration() const
    {
      return ::difftime(::time(nullptr), _t0);
    }
  };

  template <>
  class formatter<wall_timer>
  {
  public:
    template <ostream O>
    static constexpr void to(O &o, const wall_timer &timer)
    {
      format_to(o, timer.duration());
    }
  };

  namespace time
  {
    enum class zone
    {
      local,
      utc
    };
  }

  template <time::zone z>
  class date
  {
  private:
    tm _tm;

  public:
    date()
    {
      time_t _tt = ::time(nullptr);

      if constexpr (z == time::zone::local)
      {
        ::localtime_r(&_tt, &_tm);
      }
      else if constexpr (z == time::zone::utc)
      {
        ::gmtime_r(&_tt, &_tm);
      }
    }

  public:
    auto year() const
    {
      return 1900 + _tm.tm_year;
    }

    auto month() const
    {
      return _tm.tm_mon + 1;
    }

    auto yday() const
    {
      return _tm.tm_yday;
    }

    auto mday() const
    {
      return _tm.tm_mday;
    }

    auto wday() const
    {
      return _tm.tm_wday;
    }

    auto hour() const
    {
      return _tm.tm_hour;
    }

    auto min() const
    {
      return _tm.tm_min;
    }

    auto sec() const
    {
      return _tm.tm_sec;
    }
  };

  template <time::zone z>
  class formatter<date<z>>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, const date<z> &d)
    {
      format_to(o, d.mday());
      format_to(o, '/');
      format_to(o, d.month());
      format_to(o, '/');
      format_to(o, d.year());
      format_to(o, ' ');
      format_to(o, d.hour());
      format_to(o, ':');
      format_to(o, d.min());
      format_to(o, ':');
      format_to(o, d.sec());
    }
  };

  //------------------------
  //
  // XML format
  //
  //------------------------

  template <typename T>
  class xml_descriptor;

  template <typename T>
  concept xml_descriptable =
      requires(const T &t) { xml_descriptor<T>::describe(string_view<char>(), t); };

  struct xml_open_tag
  {
    string_view<char> _name;
  };

  struct xml_close_tag
  {
    string_view<char> _name;
  };

  template <typename T>
  struct xml_leaf
  {
    string_view<char> _name;
    const_reference<T> _t;
  };

  template <typename... T>
  struct xml_node
  {
    string_view<char> _name;
    tuple<T...> _nodes;
  };

  template <>
  class formatter<xml_open_tag>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, const xml_open_tag &tag)
    {
      format_to(o, '<');
      format_to(o, tag._name);
      format_to(o, '>');
    }
  };

  template <>
  class formatter<xml_close_tag>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, const xml_close_tag &tag)
    {
      format_to(o, '<');
      format_to(o, '/');
      format_to(o, tag._name);
      format_to(o, '>');
    }
  };

  template <typename T>
  class formatter<xml_leaf<T>>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, const xml_leaf<T> &leaf)
    {
      format_to(o, xml_open_tag{leaf._name});
      format_to(o, leaf._t.get());
      format_to(o, xml_close_tag{leaf._name});
    }
  };

  template <typename... T>
  class formatter<xml_node<T...>>
  {
  private:
    template <size_t I, size_t MAX, ostream O>
    constexpr static void node_to(O &o, const xml_node<T...> &node)
    {
      format_to(o, get<I>(node._nodes));

      if constexpr (I + 1 < MAX)
      {
        node_to<I + 1, MAX>(o, node);
      }
    }

  public:
    template <ostream O>
    constexpr static void to(O &o, const xml_node<T...> &node)
    {
      format_to(o, xml_open_tag{node._name});
      node_to<0, sizeof...(T)>(o, node);
      format_to(o, xml_close_tag{node._name});
    }
  };

  template <size_t N, size_t TAB, typename T>
  struct recursive_prettifier
  {
    static_assert(TAB >= 1);
    T _t;
  };

  template <size_t N, size_t TAB>
  class formatter<recursive_prettifier<N, TAB, xml_open_tag>>
  {
  public:
    template <ostream O>
    constexpr static void to(
        O &o, const recursive_prettifier<N, TAB, xml_open_tag> &tag)
    {
      if constexpr (N > 0)
      {
        for (size_t i : upto(size_t(0), N * TAB))
        {
          format_to(o, ' ');
        }
      }

      format_to(o, '<');
      format_to(o, tag._t._name);
      format_to(o, '>');
    }
  };

  template <size_t N, size_t TAB>
  class formatter<recursive_prettifier<N, TAB, xml_close_tag>>
  {
  public:
    template <ostream O>
    constexpr static void to(
        O &o, const recursive_prettifier<N, TAB, xml_close_tag> &tag)
    {
      if constexpr (N > 0)
      {
        for (size_t i : upto(size_t(0), N * TAB))
        {
          format_to(o, ' ');
        }
      }

      format_to(o, '<');
      format_to(o, '/');
      format_to(o, tag._t._name);
      format_to(o, '>');
      format_to(o, '\n');
    }
  };

  template <size_t N, size_t TAB, typename T>
  class formatter<recursive_prettifier<N, TAB, xml_leaf<T>>>
  {
  public:
    template <ostream O>
    constexpr static void to(
        O &o, const recursive_prettifier<N, TAB, xml_leaf<T>> &leaf)
    {
      format_to(o, recursive_prettifier<N, TAB, xml_open_tag>{xml_open_tag{leaf._t._name}});
      format_to(o, leaf._t._t.get());
      format_to(o, recursive_prettifier<0, TAB, xml_close_tag>{xml_close_tag{leaf._t._name}});
    }
  };

  template <size_t N, size_t TAB, typename... T>
  class formatter<recursive_prettifier<N, TAB, xml_node<T...>>>
  {
  private:
    template <size_t I, size_t MAX, ostream O>
    constexpr static void node_to(
        O &o, const recursive_prettifier<N, TAB, xml_node<T...>> &node)
    {
      format_to(o, recursive_prettifier<N + 1, TAB, rm_cvref<decltype(get<I>(node._t._nodes))>>{get<I>(node._t._nodes)});

      if constexpr (I + 1 < MAX)
      {
        node_to<I + 1, MAX>(o, node);
      }
    }

  public:
    template <ostream O>
    constexpr static void to(O &o, const recursive_prettifier<N, TAB, xml_node<T...>> &node)
    {
      format_to(o, recursive_prettifier<N, TAB, xml_open_tag>{xml_open_tag{node._t._name}});
      format_to(o, '\n');
      node_to<0, sizeof...(T)>(o, node);
      format_to(o, recursive_prettifier<N, TAB, xml_close_tag>{xml_close_tag{node._t._name}});
    }
  };

  template <typename... T>
  constexpr auto make_xml_node(
      string_view<char> name, T &&...t)
  {
    return xml_node<T...>{name, tuple<T...>{t...}};
  }

  template <character C>
  class xml_descriptor<string<C>>
  {
  public:
    constexpr static auto describe(string_view<char> name, const string<C> &s)
    {
      return xml_leaf<string<C>>{name, s};
    }
  };

  template <character C>
  class xml_descriptor<string_view<C>>
  {
  public:
    constexpr static auto describe(string_view<char> name, const string_view<C> &s)
    {
      return xml_leaf<string_view<C>>{name, s};
    }
  };

  template <integral I>
  class xml_descriptor<I>
  {
  public:
    constexpr static auto describe(string_view<char> name, const I &i)
    {
      return xml_leaf<I>{name, i};
    }
  };

  //------------------------
  //
  // Multithreading
  //
  //------------------------

  template <typename F>
  class thread
  {
  private:
    bool _joinable;
    thrd_t _thrid;
    F _call;

  private:
    static int fcall_wrapper(void *ctx)
    {
      (*static_cast<F *>(ctx))();
      return thrd_success;
    }

  public:
    ~thread() = default;

    thread(F &&f)
        : _call(forward<F>(f)),
          _joinable(false)
    {
      if (thrd_create(&_thrid, fcall_wrapper, &_call) == thrd_success)
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

  template <typename F>
  class jthread
  {
  private:
    thread<F> _t;

  public:
    ~jthread()
    {
      _t.join();
    }

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

    bool operator==(const thread<F> &o) const
    {
      return _t == o;
    }

    bool operator==(const jthread &o) const
    {
      return _t == o._t;
    }
  };

  template <typename F>
  class dthread
  {
  private:
    thread<F> _t;

  public:
    ~dthread() = default;
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

    bool operator==(const thread<F> &o) const
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

  template <time::unit T>
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
      if constexpr (T == time::unit::s)
      {
        _time.tv_sec = time;
      }
      else if constexpr (T == time::unit::ms)
      {
        _time.tv_nsec = 1'000'000 * time;
      }
      else if constexpr (T == time::unit::us)
      {
        _time.tv_nsec = 1'000 * time;
      }
      else if constexpr (T == time::unit::ns)
      {
        _time.tv_nsec = time;
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
    template <time::unit T>
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

    template <time::unit Tp>
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

  template <typename T>
  class atomic
  {
  private:
    T _t;
    mutable mutex _mtx;

  public:
    ~atomic() = default;
    atomic(const T &t) : _t(t) {}
    atomic(T &&t) : _t(transfer(t)) {}
    atomic(const atomic &) = delete;
    atomic(atomic &&) = delete;
    atomic &operator=(const atomic &) = delete;
    atomic &operator=(atomic &&) = delete;

  public:
    template <typename F>
    void apply(F &&f)
    {
      scoped_lock lck(_mtx);
      forward<F>(f)(_t);
    }

    template <typename F>
    void apply(F &&f) const
    {
      scoped_lock lck(_mtx);
      forward<F>(f)(_t);
    }
  };

  template <native_number T>
  class atomic<T>
  {
  private:
    T _t;

  public:
    ~atomic() = default;
    atomic(const T &t) : _t(t) {}
    atomic(T &&t) : _t(transfer(t)) {}
    atomic(const atomic &) = delete;
    atomic(atomic &&) = delete;
    atomic &operator=(const atomic &) = delete;
    atomic &operator=(atomic &&) = delete;

  public:
    template <typename F>
    void apply(F &&f)
    {
      forward<F>(f)(_t);
    }

    template <typename F>
    void apply(F &&f) const
    {
      forward<F>(f)(_t);
    }
  };

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

    template <mutex_type T, time::unit Tp>
    void waitfor(basic_mutex<T> &m, duration_spec<Tp> spec)
    {
      if (_useable)
      {
        cnd_timedwait(&_c, m.underlying(), &spec.spec());
      }
    }
  };

  //-----------------------
  //
  // Events utilities
  //
  //-----------------------

  enum class signal
  {
    pre_changed,
    changed,
    pre_destroyed,
    destroyed
  };

  template <typename T>
  class observer
  {
  private:
    T _t;

  private:
    function<void(const T &)> _destroyed;
    function<void(const T &)> _changed;

  public:
    ~observer()
    {
      if (!_destroyed.empty())
      {
        _destroyed(_t);
      }
    }

    observer(const T &t) : _t(t) {}
    observer(T &&t) : _t(t) {}

    observer(const observer &) = default;
    observer(observer &&) = default;

    observer &operator=(const T &t)
    {
      _t = t;

      if (!_changed.empty())
      {
        _changed(_t);
      }

      return *this;
    }

    observer &operator=(T &&t)
    {
      _t = transfer(t);

      if (!_changed.empty())
      {
        _changed(_t);
      }

      return *this;
    }

    observer &operator=(const observer &) = default;
    observer &operator=(observer &&) = default;

  public:
    template <signal S, callable<void, const int &> F>
    void slot(F &&sl)
    {
      if constexpr (S == signal::destroyed)
      {
        _destroyed = forward<F>(sl);
      }
      else if constexpr (S == signal ::changed)
      {
        _changed = forward<F>(sl);
      }
    }
  };

  template <typename T>
  class observer2;

  template <typename T>
  class observable
  {
  private:
    T _t;
    vector<observer2<T> *> _obss;

  public:
    ~observable()
    {
      notify<signal::pre_destroyed>();
    }

    observable(const T &t) : _t(t) {}
    observable(T &&t) : _t(t) {}

    observable(const observable &) = default;
    observable(observable &&) = default;

    observable &operator=(const T &t)
    {
      notify<signal::pre_changed>();
      _t = t;
      notify<signal::changed>();
      return *this;
    }

    observable &operator=(T &&t)
    {
      notify<signal::pre_changed>();
      _t = transfer(t);
      notify<signal::changed>();
      return *this;
    }

    observable &operator=(const observable &) = default;
    observable &operator=(observable &&) = default;

  private:
    template <signal S>
    void notify()
    {
      for (observer2<T> *obs : _obss)
      {
        if (obs != nullptr)
        {
          obs->template notify<S>(_t);
        }
      }
    }

    void observe(observer2<T> *obs)
    {
      _obss.push(obs);
    }

    friend class observer2<T>;
  };

  template <typename T>
  class observer2
  {
  private:
    reference<observable<T>> _obs;
    function<void(const T &)> _pre_destroyed;
    function<void(const T &)> _pre_changed;
    function<void(const T &)> _changed;

  public:
    ~observer2() = default;
    observer2(observable<T> &obs) : _obs(obs)
    {
      obs.observe(this);
    }

    observer2(const observer2 &) = delete;
    observer2(observer2 &&) = default;
    observer2 &operator=(const observer2 &) = delete;
    observer2 &operator=(observer2 &&) = default;

  public:
    template <signal S>
    void notify(const T &t)
    {

      if constexpr (S == signal::pre_changed)
      {
        if (!_pre_changed.empty())
        {
          _pre_changed(t);
        }
      }
      else if constexpr (S == signal::changed)
      {
        if (!_changed.empty())
        {
          _changed(t);
        }
      }
      else if constexpr (S == signal::pre_destroyed)
      {
        if (!_pre_destroyed.empty())
        {
          _pre_destroyed(t);
        }
      }
    }

    template <signal S, callable<void, const int &> F>
    void slot(F &&sl)
    {
      if constexpr (S == signal::pre_destroyed)
      {
        _pre_destroyed = forward<F>(sl);
      }
      else if constexpr (S == signal::pre_changed)
      {
        _pre_changed = forward<F>(sl);
      }
      else if constexpr (S == signal ::changed)
      {
        _changed = forward<F>(sl);
      }
    }
  };
}

namespace stew
{
  // class benchmark
  // {

  // };
}

#endif
