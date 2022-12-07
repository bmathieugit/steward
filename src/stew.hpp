#ifndef __stew_hpp__
#define __stew_hpp__

#include <clibs.hpp>
#include <string>

namespace stew
{
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
    template <size_t I, class T0, class... Tn>
    struct typeat
    {
      using type = typeat<I - 1, Tn...>;
    };

    template <class T0, class... Tn>
    struct typeat<0, T0, Tn...>
    {
      using type = T0;
    };
  }

  template <size_t I, class... T>
  using typeat = typename impl::typeat<I, T...>::type;

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

    template <class T>
    struct rm_const
    {
      using type = T;
    };

    template <class T>
    struct rm_const<const T>
    {
      using type = T;
    };

    template <class T>
    struct rm_volatile
    {
      using type = T;
    };

    template <class T>
    struct rm_volatile<volatile T>
    {
      using type = T;
    };
  }

  template <class T>
  using rm_const = typename impl::rm_const<T>::type;

  template <class T>
  using rm_volatile = typename impl::rm_volatile<T>::type;

  template <class T>
  using rm_ref = typename impl::rm_ref<T>::type;

  template <class T>
  using rm_cvref = rm_const<rm_volatile<rm_ref<T>>>;

  template <typename T, typename U>
  struct is_same
  {
    static constexpr bool value = false;
  };

  template <typename T>
  struct is_same<T, T>
  {
    static constexpr bool value = true;
  };

  template <typename T, typename U>
  constexpr bool is_same_v = is_same<T, U>::value;

  template <typename T, typename U>
  concept same_as = is_same_v<T, U>;

  template <typename T, typename... U>
  concept same_one_of = (same_as<T, U> || ...);

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

  template <class T>
  rm_ref<T> &&move(T &&t)
  {
    return static_cast<rm_ref<T> &&>(t);
  }

  // -----------------------------
  //
  // SWAP Functions
  //
  // -----------------------------

  template <class T>
  void swap(T &t1, T &t2)
  {
    T tmp = move(t1);
    t1 = move(t2);
    t2 = move(tmp);
  }

  //////////////
  /// RESULT ///
  //////////////

  template <class S, class E>
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

  template <range R1, range R2>
  constexpr bool starts_with(const R1 &r1, const R2 &r2)
  {
    auto b1 = r1.begin();
    auto b2 = r2.begin();

    auto e1 = r1.end();
    auto e2 = r2.end();

    while (b1 != e1 && b2 != e2 && *b1 == *b2)
    {
      ++b1;
      ++b2;
    }

    return b2 == e2;
  }

  template <range R, class T>
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

  template <class R, class P>
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

  // ---------------------------------
  //
  // VECTOR
  //
  // A vector is a contigous resizable container
  // ---------------------------------

  template <class T, unsigned_integral S = size_t>
  class fixed_vector
  {
  private:
    S _size{0};
    S _max{0};
    T *_data{nullptr};

  public:
    ~fixed_vector()
    {
      _size = 0;
      _max = 0;
      delete[] _data;
      _data = nullptr;
    }

    fixed_vector() = default;

    fixed_vector(S max) : _size{0}, _max{max}, _data{new T[_max]} {}

    fixed_vector(const fixed_vector &o)
        : fixed_vector(o._max)
    {
      for (S i{0}; i < _size; ++i)
      {
        _data[i] = o._data[i];
      }

      _size = o._size;
    }

    fixed_vector(fixed_vector &&o)
        : _size{o._size}, _max{o._max}, _data{o._data}
    {
      o._size = 0;
      o._max = 0;
      o._data = nullptr;
    }

    fixed_vector &operator=(const fixed_vector &o)
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

    fixed_vector &operator=(fixed_vector &&o)
    {
      if (this != &o)
      {
        _size = o._size;
        _max = o._max;
        _data = o._data;

        o._size = 0;
        o._max = 0;
        o._data = nullptr;
      }

      return *this;
    }

  public:
    auto begin()
    {
      return _data;
    }

    auto end()
    {
      return _data + _size;
    }

    auto begin() const
    {
      return _data;
    }

    auto end() const
    {
      return _data + _size;
    }

    T &operator[](S i)
    {
      return _data[i];
    }

    const T &operator[](S i) const
    {
      return _data[i];
    }

  public:
    bool empty() const
    {
      return _size == 0;
    }

    bool full() const
    {
      return _size == _max;
    }

    auto size() const
    {
      return _size;
    }

    auto cap() const
    {
      return _max;
    }

    operator bool() const
    {
      return empty();
    }

  public:
    void push_back(const T &t)
    {
      if (!full())
      {
        _data[_size] = t;
        _size += 1;
      }
    }

    void push_back(T &&t)
    {
      if (!full())
      {
        _data[_size] = move(t);
        _size += 1;
      }
    }

    void pop_back()
    {
      if (_size != 0)
      {
        _size -= 1;
      }
    }
  };

  template <class T, unsigned_integral S = size_t>
  class vector
  {
  private:
    fixed_vector<T, S> _data;

  public:
    void swap(vector &o)
    {
      _data.swap(o._data);
    }

  public:
    ~vector() = default;
    vector() = default;
    vector(S max) : _data{max} {}
    vector(const vector &) = default;
    vector(vector &) = default;
    vector &operator=(const vector &) = default;
    vector &operator=(vector &&) = default;

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

    T &operator[](S i)
    {
      return _data[i];
    }

    const T &operator[](S i) const
    {
      return _data[i];
    }

  public:
    auto empty() const
    {
      return _data.empty();
    }

    auto full() const
    {
      return _data.full();
    }

    auto size() const
    {
      return _data.size();
    }

    operator bool() const
    {
      return static_cast<bool>(_data);
    }

  public:
    void push_back(const T &t)
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

    void push_back(T &&t)
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

    void pop_back()
    {
      _data.pop_back();
    }
  };

  ///////////////////
  /// STRING_VIEW ///
  ///////////////////

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

    constexpr basic_string_view() = default;
    constexpr basic_string_view(const basic_string_view &) = default;
    constexpr basic_string_view(basic_string_view &&) = default;
    constexpr basic_string_view &operator=(const basic_string_view &) = default;
    constexpr basic_string_view &operator=(basic_string_view &&) = default;

  public:
    auto begin() const
    {
      return _begin;
    }

    auto end() const
    {
      return _end;
    }

    auto data() const
    {
      return _begin;
    }

  public:
    auto size() const
    {
      return _begin == nullptr ? 0 : _end - _begin;
    }

    auto empty() const
    {
      return size() == 0;
    }

    bool equals(const basic_string_view &o) const
    {
      return stew::equals(*this, o);
    }

    auto find(basic_string_view o) const
    {
      return stew::find(*this, o);
    }

    auto find(const C *c) const
    {
      return stew::find(*this, basic_string_view(c));
    }

    auto find(C c) const
    {
      return stew::find(*this, c);
    }

    auto starts_with(const basic_string_view &o) const
    {
      return stew::starts_with(*this, o);
    }

    auto contains(const basic_string_view &o) const
    {
      return stew::contains(*this, o);
    }

    auto contains(C c) const
    {
      return stew::contains(*this, c);
    }

    struct around_pair
    {
      bool _found;
      basic_string_view _bef;
      basic_string_view _aft;
    };

    around_pair around(basic_string_view sep)
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
    operator bool() const
    {
      return !empty();
    }

    bool operator==(const basic_string_view &o) const
    {
      return equals(o);
    }

    bool operator!=(const basic_string_view &o) const
    {
      return !operator==(o);
    }

    C operator[](size_t i) const
    {
      return _begin[i];
    }
  };

  using string_view = basic_string_view<char>;
  using wstring_view = basic_string_view<wchar_t>;

  string_view operator"" _sv(const char *s, size_t n)
  {
    return string_view(s, n);
  }

  wstring_view operator"" _sv(const wchar_t *s, size_t n)
  {
    return wstring_view(s, n);
  }

  //////////////
  /// STRING ///
  //////////////

  template <character C>
  class basic_string
  {
  private:
    size_t _max;
    size_t _size;
    C *_data;

  public:
    ~basic_string()
    {
      if (_data != nullptr)
      {
        delete _data;
        _data = nullptr;
        _size = 0;
        _max = 0;
      }
    }

    basic_string(size_t max)
        : _max(max < 10 ? 10 : max),
          _size(0),
          _data(new C[_max])
    {
      _data[0] = '\0';
    }

    basic_string() : basic_string(10) {}

    template <typename I>
    basic_string(I b, I e)
        : basic_string(e - b)
    {
      for (size_t i(0); i < _max; ++i)
      {
        _data[i] = b[i];
      }

      _size = _max;
    }

    basic_string(basic_string_view<C> o)
        : basic_string(o.begin(), o.end())
    {
    }

    basic_string(const C *c)
        : basic_string(basic_string_view<C>(c))
    {
    }

    basic_string(const basic_string &o)
        : basic_string(o.begin(), o.end())
    {
    }

    basic_string(basic_string &&o)
    {
      _max = o._max;
      _size = o._size;
      _data = o._data;

      o._max = 0;
      o._size = 0;
      o._data = nullptr;
    }

    basic_string &operator=(const basic_string &o)
    {
      if (this != &o)
      {
        if (_max < o._size)
        {
          delete _data;
          _max = o._max;
          _data = new C[_max];
        }

        for (size_t i(0); i < _size; ++i)
        {
          _data[i] = o._data[i];
        }

        _size = o._size;
        _data[_size] = '\0';
      }

      return *this;
    }

    basic_string &operator=(basic_string &&o)
    {
      if (this != &o)
      {
        _max = o._max;
        _data = o._data;
        _size = o._size;

        o._max = 0;
        o._size = 0;
        o._data = nullptr;
      }

      return *this;
    }

  public:
    auto begin()
    {
      return _data;
    }

    auto end()
    {
      return _data + _size;
    }

    auto begin() const
    {
      return _data;
    }

    auto end() const
    {
      return _data + _size;
    }

    auto data() const
    {
      return _data;
    }

    auto data()
    {
      return _data;
    }

  public:
    auto size() const
    {
      return _size;
    }

    auto capacity() const
    {
      return _max;
    }

    auto empty() const
    {
      return _size == 0;
    }

    auto starts_with(const basic_string &o) const
    {
      return stew::starts_with(*this, o);
    }

    auto contains(const basic_string &o) const
    {
      return stew::contains(*this, o);
    }

    auto contains(C c) const
    {
      return stew::contains(*this, c);
    }

    bool equals(const basic_string &o) const
    {
      return stew::equals(*this, o);
    }

  public:
    void push_one(C c)
    {
      if (_size == _max)
      {
        C *tmp = _data;
        _max = _max * 2;
        _data = new C[_max];

        for (size_t i(0); i < _size; ++i)
        {
          _data[i] = tmp[i];
        }

        delete tmp;
      }

      _data[_size] = c;
      ++_size;
      _data[_size] = '\0';
    }

    void push_all(basic_string_view<C> o)
    {
      for (auto c : o)
      {
        push_one(c);
      }
    }

  public:
    operator basic_string_view<C>() const
    {
      return basic_string_view<C>(begin(), end());
    }

    operator bool() const
    {
      return !empty();
    }

    bool operator==(basic_string_view<C> o) const
    {
      return equals(o);
    }

    bool operator!=(basic_string_view<C> o) const
    {
      return !operator==(o);
    }

    C &operator[](size_t i)
    {
      return _data[i];
    }

    C operator[](size_t i) const
    {
      return _data[i];
    }
  };

  using string = basic_string<char>;
  using wstring = basic_string<wchar_t>;

  //////////////
  /// FORMAT ///
  //////////////

  template <typename T>
  class formatter;

  template <typename O>
  concept ostream = requires(O &o, char c, const basic_string<char> &s) {
                      o.push_one(c);
                      o.push_all(s);
                    } || requires(O &o, wchar_t c, const basic_string<wchar_t> &s) {
                           o.push_one(c);
                           o.push_all(s);
                         };

  namespace fmt
  {
    template <ostream O, character C, typename A>
    basic_string_view<C> format_one_to(O &o, basic_string_view<C> &fmt, const A &a)
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
    void format_to(O &o, basic_string_view<C> fmt, const A &...a)
    {
      ((fmt = format_one_to(o, fmt, a)), ...);
      formatter<basic_string_view<C>>::to(o, fmt);
    }
  }

  template <ostream O, typename... A>
  void format_to(O &o, string_view fmt, const A &...a)
  {
    fmt::format_to(o, fmt, a...);
  }

  template <ostream O, typename... A>
  void format_to(O &o, wstring_view fmt, const A &...a)
  {
    fmt::format_to(o, fmt, a...);
  }

  template <character C>
  class formatter<C>
  {
  public:
    template <ostream O>
    static void to(O &os, C o)
    {
      os.push_one(o);
    }
  };

  template <character C>
  class formatter<basic_string<C>>
  {
  public:
    template <ostream O>
    static void to(O &os, basic_string_view<C> o)
    {
      os.push_all(o);
    }
  };

  template <character C>
  class formatter<basic_string_view<C>>
  {
  public:
    template <ostream O>
    static void to(O &os, basic_string_view<C> o)
    {
      os.push_all(o);
    }
  };

  template <size_t n, character C>
  class formatter<C[n]>
  {
  public:
    template <ostream O>
    static void to(O &os, basic_string_view<C> o)
    {
      os.push_all(o);
    }
  };

  template <signed_integral I>
  class formatter<I>
  {
  public:
    template <ostream O>
    static void to(O &o, I i)
    {
      class StackArray
      {
        char data[40];
        int i = -1;

      public:
        void push(char c) noexcept { data[++i] = c; }
        char pop() noexcept { return data[i--]; }
        bool empty() noexcept { return i == -1; }
      };

      bool neg = i < 0;

      I tmp = neg ? -i : i;

      StackArray tbuff;

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
    static void to(O &o, I i)
    {
      class StackArray
      {
        char data[40];
        int i = -1;

      public:
        void push(char c) noexcept { data[++i] = c; }
        char pop() noexcept { return data[i--]; }
        bool empty() noexcept { return i == -1; }
      };

      StackArray tbuff;
      auto tmp = i;

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
    static void to(O &o, bool b)
    {
      formatter<basic_string_view<char>>::to(o, b ? "true" : "false");
    }
  };

  //////////////
  /// STREAM ///
  //////////////

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
    void push_one(C c)
    {
      putc(c, _out);
    }

    void push_all(basic_string_view<C> s)
    {
      fwrite(s.begin(), sizeof(C), s.size(), _out);
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
      push_one('\n');
    }
  };

  using fostream = basic_fostream<char>;
  using wfostream = basic_fostream<wchar_t>;

  fostream cout(stdout);
  fostream cerr(stderr);

  //////////////////
  /// FILESYSTEM ///
  //////////////////

  namespace fs
  {
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

    template <class FS>
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

    template <class T>
    concept path_file = impl::path_file<T>::value;

    template <class T>
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