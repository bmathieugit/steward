#ifndef __stew_hpp__
#define __stew_hpp__

namespace stew
{
  using size_t = unsigned long long;
 
  struct position 
  {
    size_t _pos = 0;

    operator size_t() 
    {
      return _pos;
    }
  };

  struct quantity 
  {
    size_t _qty = 0;

    operator size_t ()
    {
      return _qty; 
    }
  };

  // le but de cette classe est d'offrir 
  // une vue sur une string avec un parcours
  // cadré et d'une certaine longueur. 
  template <typename C>
  class basic_string_frame
  {
  private:
    C* _begin = nullptr;
    C* _end = nullptr;
    C* _fbegin = nullptr;
    C* _fend = nullptr;
  };

  template <typename C>
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

    basic_string_view() = default;
    basic_string_view(const basic_string_view &) = default;
    basic_string_view(basic_string_view &&) = default;
    basic_string_view &operator=(const basic_string_view &) = default;
    basic_string_view &operator=(basic_string_view &&) = default;

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

    auto cbegin() const
    {
      return _begin;
    }

    auto cend() const
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

    auto starts_with(basic_string_view o) const
    {
      if (size() < o.size() || empty() || o.empty())
      {
        return false;
      }

      for (size_t i(0); i < o.size(); ++i)
      {
        if (_begin[i] != o._begin[i])
        {
          return false;
        }
      }

      return true;
    }

    auto ends_with(basic_string_view o) const
    {
      if (size() < o.size() || empty() || o.empty())
      {
        return false;
      }

      for (size_t i(0); i < o.size(); ++i)
      {
        if (_begin[size() - o.size() + i] != o._begin[i])
        {
          return false;
        }
      }

      return true;
    }

    auto contains(basic_string_view o) const
    {
      if (size() < o.size() || o.empty() || empty())
      {
        return false;
      }

      for (size_t i(0); i < _size - o._size; ++i)
      {
        if (_begin[i] == o._begin[0])
        {
          for (size_t j(0); j < o.size(); ++j)
          {
            if (_begin[i + j] != o._begin[j])
            {
              break;
            }
            else if (j == o.size())
            {
              return true;
            }
          }
        }
      }

      return false;
    }

    auto contains(C c) const
    {
      for (auto &i : *this)
      {
        if (i == c)
        {
          return true;
        }
      }

      return false;
    }

    bool equals(const basic_string &o) const
    {
      if (size() != o.size())
      {
        return false;
      }

      for (size_t i(0); i < size(); ++i)
      {
        if (_begin[i] != o._begin[i])
        {
          return false;
        }
      }

      return true;
    }

  public:
    operator bool() const
    {
      return empty();
    }

    bool operator==(const basic_string &o) const
    {
      return equals(o);
    }

    bool operator!=(const basic_string &o) const
    {
      return !operator==(o);
    }

    C operator[](size_t i) const
    {
      return _data[i];
    }

  public:
    basic_string_view substr(size_t pos, size_t n) const
    {
      if (pos >= size())
      {
        return basic_string_view();
      }
      else if (pos + n >= size())
      {
        return basic_string_view(_begin + pos, _end);
      }
      else
      {
        return basic_string_view(_begin + pos, _begin + pos + n);
      }
    }
  };

  using string_view = basic_string_view<char>;
  using wstring_view = basic_string_view<wchar_t>;

  template <typename C>
  class basic_string
  {
  private:
    size_t _max = 0;
    size_t _size = 0;
    C *_data = nullptr;

  private:
    void init(size_t max)
    {
      _max = max == 0 ? 10 : max;
      _size = 0;
      _data = new C[_max];
    }

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
    {
      init(max);
    }

    template <typename I>
    basic_string(I b, I e)
        : basic_string(e - b)
    {
      for (size_t i(0); i < _max; ++i)
      {
        _data[i] = b[i];
      }
    }

    basic_string(basic_string_view<C> o)
        : basic_string(o.begin(), o.end())
    {
    }

    basic_string(const basic_string &o)
        : basic_string(basic_string_view<C>(o.begin(), o.end()))
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
          _data = new[_max];
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

    auto cbegin() const
    {
      return _data;
    }

    auto cend() const
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
      return basic_string_view<C>(begin(), end())
          .starts_with(basic_string_view<C>(o.begin(), o.end()));
    }

    auto ends_with(const basic_string &o) const
    {
      return basic_string_view<C>(begin(), end())
          .ends_with(basic_string_view<C>(o.begin(), o.end()));
    }

    auto contains(const basic_string &o) const
    {
      return basic_string_view<C>(begin(), end())
          .containss(basic_string_view<C>(o.begin(), o.end()));
    }

    auto contains(C c) const
    {
      return basic_string_view<C>(begin(), end()).contains(c);
    }

    bool equals(const basic_string &o) const
    {
      return basic_string_view<C>(begin(), end())
          .equals(basic_string_view<C>(o.begin(), o.end()));
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

    void push_back(const basic_string &o)
    {
      if (_size == _max)
      {
        C *tmp = _data;
        _max = _max * 2 + o._size;
        _data = new C[_max];

        for (size_t i(0); i < _size; ++i)
        {
          _data[i] = tmp[i];
        }

        delete tmp;
      }

      for (size_t i(0); i < o._size; ++i)
      {
        _data[_size + i] = o._data[i];
      }

      _size = _size + o._size;
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
      return empty();
    }

    bool operator==(const basic_string &o) const
    {
      return equals(o);
    }

    bool operator!=(const basic_string &o) const
    {
      return !operator==(o);
    }

    basic_string operator+(const basic_string &o) const
    {
      basic_string n(*this);
      n.push_back(o);
      return n;
    }

    basic_string operator+(C c) const
    {
      basic_string n(*this);
      n.push_back(c);
      return n;
    }

    basic_string &operator+=(const basic_string &o)
    {
      push_back(o);
      return *this;
    }

    basic_string &operator+=(C c)
    {
      push_back(c);
      return *this;
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
}

#endif