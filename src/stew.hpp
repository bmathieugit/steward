#ifndef __stew_hpp__
#define __stew_hpp__

#include <clibs.hpp>
#include <atomic>

namespace stew
{
  using nullptr_t = decltype(nullptr);
  using size_t = unsigned long;

  ////////////////
  /// METAPROG ///
  ////////////////

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
    struct typeat
    {
      using type = typeat<I - 1, Tn...>;
    };

    template <typename T0, typename... Tn>
    struct typeat<0, T0, Tn...>
    {
      using type = T0;
    };
  }

  template <auto V>
  struct value_type
  {
    static constexpr auto value = V;
  };

  using true_type = value_type<true>;
  using false_type = value_type<false>;

  template <size_t I, typename... T>
  using typeat = typename impl::typeat<I, T...>::type;

  namespace impl
  {
    template <typename T>
    struct lvalue_reference_like : false_type
    {
    };

    template <typename T>
    struct lvalue_reference_like<T &> : true_type
    {
    };
  }

  template <typename T>
  concept lvalue_reference_like = impl::lvalue_reference_like<T>::value;

  template <typename T>
  concept not_lvalue_reference_like = (!lvalue_reference_like<T>);

  namespace impl
  {
    template <typename T>
    struct rm_ref
    {
      using type = T;
    };

    template <typename T>
    struct rm_ref<T &>
    {
      using type = T;
    };

    template <typename T>
    struct rm_ref<T &&>
    {
      using type = T;
    };

    template <typename T>
    struct rm_const
    {
      using type = T;
    };

    template <typename T>
    struct rm_const<const T>
    {
      using type = T;
    };

    template <typename T>
    struct rm_volatile
    {
      using type = T;
    };

    template <typename T>
    struct rm_volatile<volatile T>
    {
      using type = T;
    };
  }

  template <typename T>
  using rm_const = typename impl::rm_const<T>::type;

  template <typename T>
  using rm_volatile = typename impl::rm_volatile<T>::type;

  template <typename T>
  using rm_ref = typename impl::rm_ref<T>::type;

  template <typename T>
  using rm_cvref = rm_const<rm_volatile<rm_ref<T>>>;

  namespace impl
  {
    template <typename T, typename U>
    struct same_as
    {
      static constexpr bool value = false;
    };

    template <typename T>
    struct same_as<T, T>
    {
      static constexpr bool value = true;
    };
  }

  template <typename T, typename U>
  concept same_as = impl::same_as<T, U>::value;

  template <typename T, typename... U>
  concept same_one_of = (same_as<T, U> || ...);

  template <typename T, size_t N>
  concept size_more_than = (sizeof(T) > N);

  template <typename T, size_t N>
  concept size_lesseq_than = (sizeof(T) <= N);

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

  template <typename F, typename T>
  concept convertible_to =
      (!same_as<F, void> &&
       !same_as<T, void> &&
       ((requires(F f) { static_cast<T>(f); }) ||
        (requires(F f) { T(f); })));

  template <typename T>
  rm_ref<T> &&move(T &&t)
  {
    return static_cast<rm_ref<T> &&>(t);
  }

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

  //-----------------------------------
  //
  // Result
  //
  //-----------------------------------

  template <typename S, typename E>
  class result
  {
  private:
    union
    {
      S _success;
      E _error;
    };

    bool _who;

  public:
    ~result() = default;
    result() = delete;
    result(const S &s) : _success(s), _who(true) {}
    result(S &&s) : _success(s), _who(true) {}
    result(const E &e) : _error(e), _who(false) {}
    result(E &&e) : _error(e), _who(false) {}
    result(const result &) = default;
    result(result &&) = default;
    result &operator=(const result &) = default;
    result &operator=(result &&) = default;

  public:
    const S &success() const &
    {
      return _success;
    }

    S &&success() &&
    {
      return _success;
    }

    S &success() &
    {
      return _success;
    }

    const E &error() const &
    {
      return _error;
    }

    E &&error() &&
    {
      return _error;
    }

    E &error() &
    {
      return _error;
    }

  public:
    operator bool() const
    {
      return _who;
    }
  };

  struct basic_success
  {
  };

  /////////////////
  /// ALGORITHM ///
  /////////////////

  template <typename C>
  concept range = requires(C &c) {
                    c.begin();
                    c.end();
                  };

  template <typename I>
  class frame
  {
  private:
    I _begin;
    I _end;

  public:
    ~frame() = default;
    frame() = default;
    frame(I b, I e) : _begin(b), _end(e) {}
    frame(const frame &) = default;
    frame(frame &&) = default;
    frame &operator=(const frame &) = default;
    frame &operator=(frame &&) = default;

  public:
    auto begin()
    {
      return _begin;
    }

    auto end()
    {
      return _end;
    }

    auto begin() const
    {
      return _begin;
    }

    auto end() const
    {
      return _end;
    }
  };

  template <range R1, range R2>
  constexpr bool equals(const R1 &r1, const R2 &r2)
  {
    if (&r1 == &r2)
    {
      return true;
    }
    else
    {
      auto b1 = r1.begin();
      auto b2 = r2.begin();

      auto e1 = r1.end();
      auto e2 = r2.end();

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
  constexpr bool starts_with(const R1 &r1, const R2 &r2)
  {
    auto b1 = r1.begin();
    auto b2 = r2.begin();

    auto e1 = r1.end();
    auto e2 = r2.end();

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
    auto b = r.begin();
    auto e = r.end();

    return b != e && *b == t;
  }

  template <range R, typename T>
  constexpr auto find(R &r, const T &t)
  {
    auto b = r.begin();
    auto e = r.end();

    while (b != e && *b != t)
    {
      ++b;
    }

    return b;
  }

  template <range R1, range R2>
  constexpr auto find(R1 &r1, const R2 &r2)
  {
    auto b1 = r1.begin();
    auto e1 = r1.end();

    while (b1 != e1 && !starts_with(frame<decltype(b1)>(b1, e1), r2))
    {
      ++b1;
    }

    return b1;
  }

  template <range R, typename T>
  constexpr bool contains(R &r, const T &t)
  {
    return find(r, t) != r.end();
  }

  template <range R1, range R2>
  constexpr bool contains(const R1 &r1, const R2 &r2)
  {
    return find(r1, r2) != r1.end();
  }

  template <typename R, typename P>
  constexpr bool all_of(const R &r, P &&p)
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

  //---------------------------------
  //
  // Smart pointers
  //
  //---------------------------------

  template <typename T>
  class owning
  {
  private:
    T *_ptr = nullptr;

  public:
    ~owning()
    {
      if (_ptr != nullptr)
      {
        delete _ptr;
        _ptr = nullptr;
      }
    }

    owning() = default;
    owning(nullptr_t) : owning() {}

    template <typename U>
    owning(U *ptr) : _ptr(ptr) {}

    owning(const owning &) = delete;

    template <typename U>
    owning(owning<U> &&o) : _ptr(o._ptr)
    {
      o._ptr = nullptr;
    }

    owning &operator=(nullptr_t)
    {
      if (_ptr != nullptr)
      {
        delete _ptr;
      }

      return *this;
    }

    owning &operator=(const owning &) = delete;

    template <typename U>
    owning &operator=(U *ptr)
    {
      if (_ptr != ptr)
      {
        if (_ptr != nullptr)
        {
          delete _ptr;
        }

        _ptr = ptr;
      }

      return *this;
    }

    template <typename U>
    owning &operator=(owning<U> &&o)
    {
      if (this != &o)
      {
        _ptr = o._ptr;
        o._ptr = nullptr;
      }

      return *this;
    }

  public:
    T *get() const
    {
      return _ptr;
    }

  public:
    operator bool() const
    {
      return _ptr != nullptr;
    }

    auto operator*() const -> decltype(auto)
    {
      return (*_ptr);
    }

    auto operator->() const -> decltype(auto)
    {
      return _ptr;
    }
  };

  template <typename T>
  class owning<T[]>
  {
  private:
    T *_ptr = nullptr;

  public:
    ~owning()
    {
      if (_ptr != nullptr)
      {
        delete[] _ptr;
        _ptr = nullptr;
      }
    }

    owning() = default;
    owning(nullptr_t) : owning() {}

    template <typename U>
    owning(U *ptr) : _ptr(ptr) {}

    owning(const owning &) = delete;

    template <typename U>
    owning(owning<U> &&o) : _ptr(o._ptr)
    {
      o._ptr = nullptr;
    }

    owning &operator=(nullptr_t)
    {
      if (_ptr != nullptr)
      {
        delete _ptr;
      }

      return *this;
    }

    owning &operator=(const owning &) = delete;

    template <typename U>
    owning &operator=(U *ptr)
    {
      if (_ptr != ptr)
      {
        if (_ptr != nullptr)
        {
          delete[] _ptr;
        }

        _ptr = ptr;
      }

      return *this;
    }

    template <typename U>
    owning &operator=(owning<U> &&o)
    {
      if (this != &o)
      {
        _ptr = o._ptr;
        o._ptr = nullptr;
      }

      return *this;
    }

    T *get() const
    {
      return _ptr;
    }

  public:
    operator bool() const
    {
      return _ptr != nullptr;
    }

    auto operator[](size_t i) const -> decltype(auto)
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
    T *_t;

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
    const T *_t;

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
    T *_t;

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

  public:
    ~function() = default;
    function() = default;

    template <typename F>
    function(F &&f) : _handler(new function_handler<F>(forward<F>(f))) {}

    function(const function &o)
        : _handler(o._handler == nullptr ? nullptr : o._handler->clone())
    {
    }

    function(function &&o)
        : _handler(move(o._handler))
    {
      o._handler = nullptr;
    }

    template <typename F>
    function &operator=(F &&f)
    {
      _handler = new function_handler<F>(forward<F>(f));
      return *this;
    }

    function &operator=(const function &o)
    {
      if (this != &o)
      {
        _handler = o._handler == nullptr ? nullptr : o._handler->clone();
      }

      return *this;
    }

    function &operator=(function &&o)
    {
      if (this != &o)
      {
        _handler = move(o._handler);
        o._handler = nullptr;
      }

      return *this;
    }

  public:
    operator bool() const
    {
      return static_cast<bool>(_handler);
    }

    template <typename... T>
    R operator()(T &&...t)
    {
      return _handler->invoke(forward<T>(t)...);
    }
  };

  //----------------------------------
  //
  // Functionnal utilities
  //
  //----------------------------------

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

  template <typename T>
  struct is_placeholder
  {
    static constexpr bool value = false;
  };

  template <size_t N>
  struct is_placeholder<placeholder<N>>
  {
    static constexpr bool value = true;
  };

  template <typename T>
  concept placeholder_like = is_placeholder<T>::value;

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
  constexpr auto operator+(P0 p0, P1 p1) -> decltype(auto)
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
    constexpr tuple(T0 &&t0, Tn &&...tn)
        : _t{forward<T0>(t0)},
          tuple<Tn...>(forward<Tn>(tn)...)
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

  // ---------------------------------
  //
  // Array container
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

  template <typename T, size_t N>
  class stack_array
  {
  private:
    array<T, N> _data;
    int _idx = -1;

  public:
    constexpr void push(const T &c)
    {
      _data[++_idx] = c;
    }

    constexpr void push(T &&c)
    {
      _data[++_idx] = move(c);
    }

    constexpr T &&pop()
    {
      return move(_data[_idx--]);
    }

    constexpr bool empty() const
    {
      return _idx == -1;
    }
  };

  // ---------------------------------
  //
  // Vector containers
  //
  // ---------------------------------

  template <typename T, unsigned_integral S = size_t>
  class fixed_vector
  {
  private:
    S _size{0};
    S _max{0};
    owning<T[]> _data;

  public:
    constexpr ~fixed_vector() = default;
    constexpr fixed_vector() = default;

    constexpr fixed_vector(S max) : _size{0}, _max{max}, _data{new T[_max]{}}
    {
    }

    constexpr fixed_vector(const fixed_vector &o)
        : fixed_vector(o._max)
    {
      for (S i{0}; i < _size; ++i)
      {
        _data[i] = o._data[i];
      }

      _size = o._size;
    }

    constexpr fixed_vector(fixed_vector &&o)
        : _size{o._size}, _max{o._max}, _data{move(o._data)}
    {
      o._size = 0;
      o._max = 0;
      o._data = nullptr;
    }

    constexpr fixed_vector &operator=(const fixed_vector &o)
    {
      if (this != &o)
      {
        _size = o._size;
        _max = o._max;
        _data = new T[_max];

        for (S i{0}; i < _size; ++i)
        {
          _data[i] = o._data[i];
        }
      }

      return *this;
    }

    constexpr fixed_vector &operator=(fixed_vector &&o)
    {
      if (this != &o)
      {
        _size = o._size;
        _max = o._max;
        _data = move(o._data);

        o._size = 0;
        o._max = 0;
        o._data = nullptr;
      }

      return *this;
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

    constexpr T &operator[](S i)
    {
      return _data[i];
    }

    constexpr const T &operator[](S i) const
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

    constexpr auto cap() const
    {
      return _max;
    }

    constexpr operator bool() const
    {
      return !empty();
    }

  public:
    constexpr void push_back(const T &t)
    {
      if (!full())
      {
        _data[_size] = t;
        _size += 1;
      }
    }

    constexpr void push_back(T &&t)
    {
      if (!full())
      {
        _data[_size] = move(t);
        _size += 1;
      }
    }

    constexpr void pop_back()
    {
      if (_size != 0)
      {
        _size -= 1;
      }
    }
  };

  template <typename T1, typename S1, typename T2, typename S2>
  constexpr bool operator==(const fixed_vector<T1, S1> &fv1,
                            const fixed_vector<T2, S2> &fv2)
  {
    return equals(fv1, fv2);
  }

  template <typename T1, typename S1, typename T2, typename S2>
  constexpr bool operator!=(const fixed_vector<T1, S1> &fv1,
                            const fixed_vector<T2, S2> &fv2)
  {
    return !(fv1 == fv2);
  }

  template <typename T, unsigned_integral S = size_t>
  class vector
  {
  private:
    fixed_vector<T, S> _data;

  public:
    constexpr ~vector() = default;
    constexpr vector() = default;
    constexpr vector(S max) : _data{max} {}
    constexpr vector(const vector &) = default;
    constexpr vector(vector &) = default;
    constexpr vector &operator=(const vector &) = default;
    constexpr vector &operator=(vector &&) = default;

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

    constexpr T &operator[](S i)
    {
      return _data[i];
    }

    constexpr const T &operator[](S i) const
    {
      return _data[i];
    }

  public:
    constexpr auto empty() const
    {
      return _data.empty();
    }

    constexpr auto full() const
    {
      return _data.full();
    }

    constexpr auto size() const
    {
      return _data.size();
    }

    constexpr auto cap() const
    {
      return _data.cap();
    }

    constexpr operator bool() const
    {
      return static_cast<bool>(_data);
    }

  public:
    constexpr void push_back(const T &t)
    {
      if (_data.full())
      {
        fixed_vector<T, S> tmp(move(_data));
        _data = fixed_vector<T, S>(tmp.size() * 2);

        for (T &i : move(tmp))
        {
          _data.push_back(move(i));
        }
      }

      _data.push_back(t);
    }

    constexpr void push_back(T &&t)
    {
      if (_data.full())
      {
        fixed_vector<T, S> tmp(move(_data));
        _data = fixed_vector<T, S>(tmp.size() * 2);

        for (T &i : move(tmp))
        {
          _data.push_back(move(i));
        }
      }

      _data.push_back(move(t));
    }

    constexpr void pop_back()
    {
      _data.pop_back();
    }
  };

  template <typename T1, typename S1, typename T2, typename S2>
  constexpr bool operator==(const vector<T1, S1> &fv1, const vector<T2, S2> &fv2)
  {
    return equals(fv1, fv2);
  }

  template <typename T1, typename S1, typename T2, typename S2>
  constexpr bool operator!=(const vector<T1, S1> &fv1, const vector<T2, S2> &fv2)
  {
    return !(fv1 == fv2);
  }

  //--------------------------
  //
  // String view
  //
  //--------------------------

  template <character C>
  class basic_string_view
  {
  private:
    const C *_begin = nullptr;
    const C *_end = nullptr;

  public:
    constexpr ~basic_string_view() = default;

    constexpr basic_string_view(const C *b, const C *e)
    {
      _begin = b;
      _end = e;
    }

    constexpr basic_string_view(const C *b, size_t s)
    {
      _begin = b;
      _end = b + s;
    }

    constexpr basic_string_view(const C *c)
        : basic_string_view(c, strlen(c))
    {
    }

    template <size_t N>
    constexpr basic_string_view(const C (&s)[N])
        : basic_string_view(s, N)
    {
    }

    constexpr basic_string_view() = default;
    constexpr basic_string_view(const basic_string_view &) = default;
    constexpr basic_string_view(basic_string_view &&) = default;
    constexpr basic_string_view &operator=(const basic_string_view &) = default;
    constexpr basic_string_view &operator=(basic_string_view &&) = default;

  public:
    constexpr auto begin() const
    {
      return _begin;
    }

    constexpr auto end() const
    {
      return _end;
    }

    constexpr auto data() const
    {
      return _begin;
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

    constexpr bool equals(const basic_string_view &o) const
    {
      return stew::equals(*this, o);
    }

    constexpr auto find(basic_string_view o) const
    {
      return stew::find(*this, o);
    }

    constexpr auto find(const C *c) const
    {
      return stew::find(*this, basic_string_view(c));
    }

    constexpr auto find(C c) const
    {
      return stew::find(*this, c);
    }

    constexpr auto starts_with(const basic_string_view &o) const
    {
      return stew::starts_with(*this, o);
    }

    constexpr auto contains(const basic_string_view &o) const
    {
      return stew::contains(*this, o);
    }

    constexpr auto contains(C c) const
    {
      return stew::contains(*this, c);
    }

    struct around_pair
    {
      bool _found;
      basic_string_view _bef;
      basic_string_view _aft;
    };

    constexpr around_pair around(basic_string_view sep)
    {
      auto pos = find(sep);

      if (pos != _end)
      {
        return {true, {_begin, pos}, {pos + 2, _end}};
      }
      else
      {
        return {false, {_begin, _begin}, {_begin, _end}};
      }
    }

  public:
    constexpr operator bool() const
    {
      return !empty();
    }

    constexpr bool operator==(const basic_string_view &o) const
    {
      return equals(o);
    }

    constexpr bool operator!=(const basic_string_view &o) const
    {
      return !operator==(o);
    }

    constexpr C operator[](size_t i) const
    {
      return _begin[i];
    }
  };

  using string_view = basic_string_view<char>;
  using wstring_view = basic_string_view<wchar_t>;

  constexpr string_view operator"" _sv(const char *s, size_t n)
  {
    return string_view(s, n);
  }

  constexpr wstring_view operator"" _sv(const wchar_t *s, size_t n)
  {
    return wstring_view(s, n);
  }

  template <typename S, typename C>
  concept string_view_castable =
      character<C> &&
      requires(const S &s) {
        static_cast<basic_string_view<C>>(s);
      };

  template <typename S, typename C>
  concept string_view_buildable =
      character<C> &&
      requires(const S &s) {
        basic_string_view<C>(s);
      };

  template <typename S, typename C>
  concept string_view_like =
      string_view_buildable<S, C> ||
      string_view_castable<S, C>;

  //----------------------------
  //
  // String classes
  //
  //----------------------------

  template <character C>
  class basic_fixed_string
  {
  private:
    fixed_vector<C> _data;

  public:
    constexpr ~basic_fixed_string() = default;

    constexpr basic_fixed_string(size_t max) : _data(max)
    {
    }

    constexpr basic_fixed_string() : basic_fixed_string(10) {}

    constexpr basic_fixed_string(const string_view_like<C> auto &o)
        : basic_fixed_string(basic_string_view<C>(o).size())
    {
      for (C c : basic_string_view<C>(o))
      {
        _data.push_back(c);
      }
    }

    constexpr basic_fixed_string(const basic_fixed_string &o) = default;
    constexpr basic_fixed_string(basic_fixed_string &&o) = default;
    constexpr basic_fixed_string &operator=(const basic_fixed_string &o) = default;
    constexpr basic_fixed_string &operator=(basic_fixed_string &&o) = default;

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
    constexpr auto size() const
    {
      return _data.size();
    }

    constexpr auto cap() const
    {
      return _data.cap();
    }

    constexpr auto empty() const
    {
      return _data.empty();
    }

    constexpr auto full() const
    {
      return _data.full();
    }

    constexpr auto starts_with(const string_view_like<C> auto &o) const
    {
      return stew::starts_with(*this, basic_string_view<C>(o));
    }

    constexpr auto contains(const string_view_like<C> auto &o) const
    {
      return stew::contains(*this, basic_string_view<C>(o));
    }

    constexpr auto contains(C c) const
    {
      return stew::contains(*this, c);
    }

    constexpr bool equals(const string_view_like<C> auto &o) const
    {
      return stew::equals(*this, basic_string_view<C>(o));
    }

  public:
    constexpr void push_back(C c)
    {
      _data.push_back(c);
    }

    constexpr void push_back(const string_view_like<C> auto o)
    {
      for (C c : basic_string_view<C>(o))
      {
        _data.push_back(c);
      }
    }

    constexpr void pop_back()
    {
      _data.pop_back();
    }

  public:
    constexpr operator basic_string_view<C>() const
    {
      return basic_string_view<C>(begin(), end());
    }

    constexpr operator bool() const
    {
      return !empty();
    }

    constexpr bool operator==(const string_view_like<C> auto o) const
    {
      return equals(o);
    }

    constexpr bool operator!=(const string_view_like<C> auto o) const
    {
      return !operator==(o);
    }

    constexpr C &operator[](size_t i)
    {
      return _data[i];
    }

    constexpr C operator[](size_t i) const
    {
      return _data[i];
    }
  };

  using fstring = basic_fixed_string<char>;
  using wfstring = basic_fixed_string<wchar_t>;

  template <character C>
  class basic_string
  {
  private:
    vector<C> _data;

  public:
    constexpr ~basic_string() = default;

    constexpr basic_string(size_t max) : _data(max)
    {
    }

    constexpr basic_string() : basic_string(10) {}

    constexpr basic_string(const string_view_like<C> auto &o)
        : basic_string(basic_string_view<C>(o).size())
    {
      for (C c : basic_string_view<C>(o))
      {
        _data.push_back(c);
      }
    }

    constexpr basic_string(const basic_string &o) = default;
    constexpr basic_string(basic_string &&o) = default;
    constexpr basic_string &operator=(const basic_string &o) = default;
    constexpr basic_string &operator=(basic_string &&o) = default;

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
    constexpr auto size() const
    {
      return _data.size();
    }

    constexpr auto capacity() const
    {
      return _data.cap();
    }

    constexpr auto empty() const
    {
      return _data.empty();
    }

    constexpr auto full() const
    {
      return _data.full();
    }

    constexpr auto starts_with(const string_view_like<C> auto &o) const
    {
      return stew::starts_with(*this, basic_string_view<C>(o));
    }

    constexpr auto contains(const string_view_like<C> auto &o) const
    {
      return stew::contains(*this, basic_string_view<C>(o));
    }

    constexpr auto contains(C c) const
    {
      return stew::contains(*this, c);
    }

    constexpr bool equals(const string_view_like<C> auto &o) const
    {
      return stew::equals(*this, basic_string_view<C>(o));
    }

  public:
    constexpr void push_back(C c)
    {
      _data.push_back(c);
    }

    constexpr void push_back(const string_view_like<C> auto &o)
    {
      for (C c : basic_string_view<C>(o))
      {
        _data.push_back(c);
      }
    }

    constexpr void pop_back()
    {
      _data.pop_back();
    }

  public:
    constexpr operator basic_string_view<C>() const
    {
      return basic_string_view<C>(begin(), end());
    }

    constexpr operator bool() const
    {
      return !empty();
    }

    constexpr bool operator==(const string_view_like<C> auto &o) const
    {
      return equals(o);
    }

    constexpr bool operator!=(const string_view_like<C> auto &o) const
    {
      return !operator==(o);
    }

    constexpr C &operator[](size_t i)
    {
      return _data[i];
    }

    constexpr C operator[](size_t i) const
    {
      return _data[i];
    }
  };

  using string = basic_string<char>;
  using wstring = basic_string<wchar_t>;

  //------------------------------
  //
  // Formatting
  //
  //------------------------------

  template <typename T>
  class formatter;

  template <typename O>
  concept char_ostream =
      requires(O &o, char c, const basic_string<char> &s) {
        o.push_back(c);
        o.push_back(s);
      };

  template <typename O>
  concept wchar_ostream =
      requires(O &o, wchar_t c, const basic_string<wchar_t> &s) {
        o.push_back(c);
        o.push_back(s);
      };

  template <typename O>
  concept ostream = char_ostream<O> || wchar_ostream<O>;

  namespace fmt
  {
    template <ostream O, character C, typename A>
    constexpr basic_string_view<C> format_one_to(O &o, basic_string_view<C> &fmt, const A &a)
    {
      auto [found, bef, aft] = fmt.around("{}");

      if (found)
      {
        formatter<basic_string_view<C>>::to(o, bef);
        formatter<A>::to(o, a);
      }

      return aft;
    }

    template <ostream O, character C, typename... A>
    constexpr void format_to(O &o, basic_string_view<C> fmt, const A &...a)
    {
      ((fmt = format_one_to(o, fmt, a)), ...);

      if (!fmt.empty())
      {
        formatter<basic_string_view<C>>::to(o, fmt);
      };
    }
  }

  template <ostream O, typename... A>
  constexpr void format_to(O &o, string_view fmt, const A &...a)
  {
    fmt::format_to(o, fmt, a...);
  }

  template <ostream O, typename... A>
  constexpr void format_to(O &o, wstring_view fmt, const A &...a)
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

  template <string_view_like<char> S>
  class formatter<S>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, const S &o)
    {
      os.push_back(o);
    }
  };

  template <string_view_like<wchar_t> S>
  class formatter<S>
  {
  public:
    template <ostream O>
    constexpr static void to(O &os, const S &o)
    {
      os.push_back(o);
    }
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

      stack_array<char, 40> tbuff;

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

      while (!tbuff.empty())
      {
        formatter<char>::to(o, tbuff.pop());
      }
    }
  };

  template <unsigned_integral I>
  class formatter<I>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, I i)
    {
      stack_array<char, 40> tbuff;
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

      while (!tbuff.empty())
      {
        formatter<char>::to(o, tbuff.pop());
      }
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

    void push_back(const string_view_like<C> auto &s)
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
    void printf(basic_string_view<C> fmt, const T &...t)
    {
      format_to(*this, fmt, t...);
    }

    template <typename... T>
    void printfln(basic_string_view<C> fmt, const T &...t)
    {
      printf(fmt, t...);
      push_back('\n');
    }
  };

  using fostream = basic_fostream<char>;
  using wfostream = basic_fostream<wchar_t>;

  fostream cout(stdout);
  fostream cerr(stderr);

  //-----------------------
  //
  // Filesystem
  //
  //-----------------------

  namespace fs
  {
    template <typename T>
    struct fs_result;

    template <character C>
    fs_result<basic_string<C>> current_dir();

    template <character C>
    fs_result<void> remove_dir(const string_view_like<C> auto &dirname, bool recursive);

    template <character C>
    fs_result<void> make_dir(const string_view_like<C> auto &dirname, bool recursive);

    template <character C>
    fs_result<void> remove_file(const string_view_like<C> auto &filename);

    template <character C>
    fs_result<void> make_file(const string_view_like<C> auto &filename);

    template <character C>
    fs_result<void> rename_file(const string_view_like<C> auto &oldname,
                                const string_view_like<C> auto &newname);

    template <character C>
    fs_result<void> rename_dir(const string_view_like<C> auto &oldname,
                               const string_view_like<C> auto &newname);

    template <character C>
    fs_result<size_t> file_size(const string_view_like<C> auto &filename);

    template <character C>
    fs_result<bool> file_exists(const string_view_like<C> auto &filename);

    template <character C>
    fs_result<bool> dir_exists(const string_view_like<C> auto &dirname);

    template <character C>
    fs_result<void> dir_copy(const string_view_like<C> auto &oldname,
                             const string_view_like<C> auto &newname);

    template <character C>
    fs_result<void> file_copy(const string_view_like<C> auto &oldname,
                              const string_view_like<C> auto &newname);

    template <character C>
    fs_result<bool> is_dir(const string_view_like<C> auto &dirname);

    template <character C>
    fs_result<bool> is_file(const string_view_like<C> auto &filename);

    enum class permission : size_t
    {
      rwx = R_OK | W_OK | X_OK,
      rw = R_OK | W_OK,
      rx = R_OK | X_OK,
      r = R_OK,
      wx = W_OK | X_OK,
      w = W_OK,
      x = X_OK,
      f = F_OK
    };

    using perm = permission;

    class mode
    {
    private:
      perm _user = perm::f;
      perm _group = perm::f;
      perm _other = perm::f;

      size_t _literal = to_literal();

    public:
      ~mode() = default;
      mode() = default;
      mode(perm u, perm g, perm o)
          : _user(u), _group(g),
            _other(o), _literal(to_literal()) {}
      mode(const mode &) = default;
      mode(mode &&) = default;
      mode &operator=(const mode &) = default;
      mode &operator=(mode &&) = default;

    public:
      operator size_t() const
      {
        return _literal;
      }

    private:
      size_t to_literal()
      {
        return static_cast<size_t>(_user) << 6 |
               static_cast<size_t>(_group) << 3 |
               static_cast<size_t>(_other);
      }
    };

    template <typename P>
    concept path = requires(const P &p) {
                     p.path();
                     p.perms();
                   };

    template <typename FS>
    class directory
    {
    private:
      string _path;
      mode _mode;

    public:
      ~directory() = default;
      directory() = delete;
      directory(string_view path, mode m = {perm::rwx, perm::rx, perm::rx})
          : _path(path), _mode(m) {}
      directory(const directory &) = default;
      directory(directory &&) = default;
      directory &operator=(const directory &) = default;
      directory &operator=(directory &&) = default;

    public:
      const string &path() const
      {
        return _path;
      }

      const mode &perms() const
      {
        return _mode;
      }
    };

    namespace literatals
    {
      directory<void> operator"" _dp(const char *s, size_t n)
      {
        return directory<void>(string_view(s, n));
      }
    }

    template <typename FS>
    class file
    {
    private:
      string _path;

      mode _mode;

    public:
      ~file() = default;
      file() = delete;
      file(string_view path, mode m = {perm::rwx, perm::rx, perm::rx})
          : _path(path), _mode(m) {}
      file(const file &) = default;
      file(file &&) = default;
      file &operator=(const file &) = default;
      file &operator=(file &&) = default;

    public:
      const string &path() const
      {
        return _path;
      }

      const mode &perms() const
      {
        return _mode;
      }
    };

    namespace literatals
    {
      file<void> operator"" _fp(const char *s, size_t n)
      {
        return file<void>(string_view(s, n));
      }
    }

    template <path P>
    class cpath
    {
    private:
      string_view _path;
      mode _mode;

    public:
      ~cpath() = default;
      cpath() = default;
      cpath(string_view p, mode m = {perm::rwx, perm::rx, perm::rx}) : _path(p), _mode(m) {}
      cpath(const cpath &) = default;
      cpath(cpath &&) = default;
      cpath &operator=(const cpath &) = default;
      cpath &operator=(cpath &&) = default;

    public:
      const string_view &path() const
      {
        return _path;
      }

      const mode &perms() const
      {
        return _mode;
      }
    };

    namespace literatals
    {
      cpath<file<void>> operator"" _cfp(const char *s, size_t n)
      {
        return cpath<file<void>>(string_view(s, n));
      }

      cpath<directory<void>> operator"" _cdp(const char *s, size_t n)
      {
        return cpath<directory<void>>(string_view(s, n));
      }
    }

    namespace impl
    {
      template <typename T>
      struct path_file
      {
        static constexpr bool value = false;
      };

      template <typename FS>
      struct path_file<file<FS>>
      {
        static constexpr bool value = true;
      };

      template <typename FS>
      struct path_file<cpath<file<FS>>>
      {
        static constexpr bool value = true;
      };

      template <typename T>
      struct path_directory
      {
        static constexpr bool value = false;
      };

      template <typename FS>
      struct path_directory<directory<FS>>
      {
        static constexpr bool value = true;
      };

      template <typename FS>
      struct path_directory<cpath<directory<FS>>>
      {
        static constexpr bool value = true;
      };
    }

    template <typename T>
    concept path_file = impl::path_file<T>::value;

    template <typename T>
    concept path_directory = impl::path_directory<T>::value;

    struct ferror
    {
    };

    template <path P>
    bool fexists(const P &p)
    {
      return stew::c::access(p.path().data(), (int)perm::f) == 0;
    }

    template <path P>
    bool freadable(const P &p)
    {
      return stew::c::access(p.path().data(), (int)perm::r) == 0;
    }

    template <path P>
    bool fwritable(const P &p)
    {
      return stew::c::access(p.path().data(), (int)perm::w) == 0;
    }

    template <path P>
    bool fexecutable(const P &p)
    {
      return stew::c::access(p.path().data(), (int)perm::x) == 0;
    }

    // ---------------------------------------------------------
    //
    // fcreate : create a file corresponding to the path specification.
    //
    // ---------------------------------------------------------

    template <path P>
    result<basic_success, ferror> fcreate(const P &p)
    {
      if constexpr (path_directory<P>)
      {
        auto res = stew::c::mkdir(p.path().data(), static_cast<size_t>(p.perms()));

        if (res == 0 || res == EEXIST)
        {
          return basic_success();
        }
      }
      else if constexpr (path_file<P>)
      {
        auto res = stew::c::touch(p.path().data(), static_cast<size_t>(p.perms()));

        if (res == 0 || res == EEXIST)
        {
          return basic_success();
        }
      }

      return ferror();
    }

    template <path P>
    result<basic_success, ferror> fremove(const P &p)
    {
      if (stew::c::remove(p.path().data()) == 0)
      {
        return basic_success();
      }
      else
      {
        return ferror();
      }
    }

    template <path P>
    result<basic_success, ferror> frename(const P &pold, const P &pnew)
    {
      if (stew::c::rename(pold.path().data(), pnew.path().data()) == 0)
      {
        return basic_success();
      }
      else
      {
        return ferror();
      }
    }

    template <path P>
    result<basic_success, ferror> fchmod(const P &p, mode m)
    {
      if (stew::c::chmod(p.path().data(), static_cast<size_t>(m)) == 0)
      {
        return basic_success();
      }
      else
      {
        return ferror();
      }
    }

  }
}

#endif