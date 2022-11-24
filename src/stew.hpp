#ifndef __stew_hpp__
#define __stew_hpp__

#include <cstring>
#include <cstdio>

namespace stew
{
  using size_t = unsigned long long;

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

  template <typename C>
  concept range = requires(C &c)
  {
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

  struct position
  {
    size_t _pos = 0;

    operator size_t()
    {
      return _pos;
    }
  };

  constexpr position pos(size_t p) noexcept
  {
    return position{p};
  }

  struct quantity
  {
    size_t _qty = 0;

    operator size_t()
    {
      return _qty;
    }
  };

  constexpr quantity qty(size_t q) noexcept
  {
    return quantity{q};
  }

  template <character C>
  class basic_string_view
  {
  private:
    const C *_begin = nullptr;
    const C *_end = nullptr;

  public:
    ~basic_string_view() = default;

    basic_string_view(const C *b, const C *e)
    {
      _begin = b;
      _end = e;
    }

    basic_string_view(const C *b, size_t s)
    {
      _begin = b;
      _end = b + s;
    }

    basic_string_view(const C *c)
        : basic_string_view(c, strlen(c))
    {
    }

    basic_string_view() = default;
    basic_string_view(const basic_string_view &) = default;
    basic_string_view(basic_string_view &&) = default;
    basic_string_view &operator=(const basic_string_view &) = default;
    basic_string_view &operator=(basic_string_view &&) = default;

  public:
    auto begin() const
    {
      return _begin;
    }

    auto end() const
    {
      return _end;
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

    basic_string_view substr(position pos, quantity n) const
    {
      const C *b = nullptr;
      const C *e = nullptr;
      auto s = size();

      if (pos <= s)
      {
        if (pos + n <= s)
        {
          b = _begin + pos;
          e = _end;
        }
        else
        {
          b = _begin + pos;
          e = _begin + pos + n;
        }
      }

      return basic_string_view(b, e);
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

  template <character C>
  class basic_string
  {
  private:
    size_t _max = 0;
    size_t _size = 0;
    C *_data = nullptr;

  public:
    ~basic_string()
    {
      if (_data != nullptr)
      {
        delete _data;
        _size = 0;
        _max = 0;
      }
    }

    basic_string() = default;

    basic_string(size_t max)
        : _max(max == 0 ? 10 : max),
          _size(0),
          _data(new C[_max])
    {
    }

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
    void push_back(C c)
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
    }

    void push_back(basic_string_view<C> o)
    {
      for (auto c : o)
      {
        push_back(c);
      }
    }

    void push_back(const basic_string &o)
    {
      push_back(static_cast<basic_string_view<C>>(o));
    }

    basic_string substr(size_t pos, size_t n) const
    {
      return basic_string(basic_string_view<C>(begin(), end()).substr(pos, n));
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

  template <typename T>
  class formatter;

  template <typename O, typename C>
  concept format_output = character<C> && requires(O &o, C c, const basic_string<C> &s)
  {
    o.push_back(c);
    o.push_back(s);
  };

  namespace fmt
  {
    template <character C, format_output<C> O, typename H, typename... T>
    void format_impl(basic_string_view<C> fmt, O &s, const H &h, const T &...t)
    {
      auto i = fmt.find("{}");

      formatter<basic_string_view<C>>::to(s, basic_string_view<C>(fmt.begin(), i));

      if (i != fmt.end())
      {
        formatter<H>::to(s, h);

        basic_string_view<C> tail(i + 2, fmt.end());

        if constexpr (sizeof...(t) > 0)
        {
          format_impl(tail, s, t...);
        }
        else
        {
          formatter<basic_string_view<C>>::to(s, tail);
        }
      }
    }

    template <character C, typename... A>
    basic_string<C> format(basic_string_view<C> fmt, const A &...a)
    {
      basic_string<C> s(fmt.size());

      if constexpr (sizeof...(a) > 0)
      {
        format_impl(fmt, s, a...);
      }
      else
      {
        formatter<basic_string_view<C>>::to(s, fmt);
      }

      return s;
    }
  }

  template <typename... A>
  string format(string_view fmt, const A &...a)
  {
    return fmt::format(fmt, a...);
  }

  template <typename... A>
  wstring format(wstring_view fmt, const A &...a)
  {
    return fmt::format(fmt, a...);
  }

  template <character C>
  class formatter<basic_string<C>>
  {
  public:
    template <format_output<C> O>
    static void to(O &os, const basic_string<C> &o)
    {
      os.push_back(o);
    }
  };

  template <character C>
  class formatter<basic_string_view<C>>
  {
  public:
    template <format_output<C> O>
    static void to(O &os, basic_string_view<C> o)
    {
      os.push_back(o);
    }
  };

  template <character C>
  class formatter<C>
  {
  public:
    template <format_output<C> O>
    static void to(O &os, C o)
    {
      os.push_back(o);
    }
  };

  template <signed_integral I>
  class formatter<I>
  {
  public:
    template <format_output<char> O>
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
    template <format_output<char> O>
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
    template <format_output<char> O>
    static void to(O &o, bool b)
    {
      formatter<basic_string_view<char>>::to(o, b ? "true" : "false");
    }
  };

  template <character C>
  class basic_fostream
  {
  private:
    std::FILE *_out = nullptr;

  public:
    ~basic_fostream()
    {
      if (_out != nullptr)
      {
        std::fclose(_out);
        _out = nullptr;
      }
    }

    basic_fostream() = default;

    basic_fostream(const C *path)
        : _out(std::fopen(path, "w"))
    {
    }

    basic_fostream(std::FILE *o)
        : _out(o)
    {
    }

    basic_fostream(const basic_fostream &) = delete;
    basic_fostream(basic_fostream &&) = default;
    basic_fostream &operator=(const basic_fostream &) = delete;
    basic_fostream &operator=(basic_fostream &) = default;

  public:
    void write(basic_string_view<C> s)
    {
      std::fwrite(s.begin(), sizeof(C), s.size(), _out);
    }

    void flush()
    {
      std::fflush(_out);
    }

    void close()
    {
      std::fclose(_out);
      _out = nullptr;
    }

  public:
    template <typename... T>
    void printf(basic_string_view<C> fmt, const T &...t)
    {
    }
  };

  using fostream = basic_fostream<char>;
  using wfostream = basic_fostream<wchar_t>;

  fostream cout(stdout);
  fostream cerr(stderr);
}

#endif