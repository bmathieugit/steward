#ifndef __stew_dbfile_hpp__
#define __stew_dbfile_hpp__

#include <any>
#include <map>
#include <string>
#include <variant>
#include <optional>

namespace stew::dbf::storage::mem
{
  class leaf
  {
    std::string _name;
    std::any _data;

  public:
    ~leaf() = default;
    leaf() = default;
    leaf(const std::string &name) : _name(name) {}
    leaf(std::string &&name) : _name(name) {}
    template <typename T>
    leaf(const std::string &name, const T &data)
        : _name(name), _data(data) {}
    template <typename T>
    leaf(std::string &&name, const T &data)
        : _name(name), _data(data) {}
    template <typename T>
    leaf(const std::string &name, T &&data)
        : _name(name), _data(data) {}
    template <typename T>
    leaf(std::string &&name, T &&data)
        : _name(name), _data(data) {}
    leaf(const leaf &) = default;
    leaf(leaf &&) = default;
    leaf &operator=(const leaf &) = default;
    leaf &operator=(leaf &&) = default;

  public:
    const std::string &name() const
    {
      return _name;
    }

    void name(const std::string &n)
    {
      _name = n;
    }

    void name(std::string &&n)
    {
      _name = n;
    }

    const std::any &data() const
    {
      return _data;
    }

    template <typename T>
    void data(T &&data)
    {
      _data = data;
    }

    template <typename T>
    void data(const T &data)
    {
      _data = data;
    }
  };

  class root
  {
    using node = std::variant<root, leaf>;

    std::string _name;
    std::map<std::string, node> _childs;

  public:
    ~root() = default;
    root() = default;
    root(const std::string &name) : _name(name) {}
    root(std::string &&name) : _name(name) {}
    root(const root &) = default;
    root(root &&) = default;
    root &operator=(const root &) = default;
    root &operator=(root &&) = default;

  public:
    const std::string &name() const
    {
      return _name;
    }

    const auto &childs() const
    {
      return _childs;
    }

  public:
    template <typename T, typename... S>
    bool insert(T &&data, const std::string &pth, const S &...s)
    {
      if (_childs.contains(pth))
      {
        if constexpr (sizeof...(s) == 0)
        {
          if (_childs[pth].index() == 1)
          {
            std::get<leaf>(_childs[pth]).data(data);
            return true;
          }
        }
        else
        {
          return std::get<root>(_childs[pth]).insert(data, s...);
        }
      }
      else
      {
        if constexpr (sizeof...(s) == 0)
        {
          _childs[pth] = leaf(pth, data);
          return true;
        }
        else
        {
          _childs[pth] = root(pth);
          return std::get<root>(_childs[pth]).insert(data, s...);
        }
      }

      return false;
    }

    template <typename T, typename... S>
    bool insert(const T &data, const std::string &pth, S... s)
    {
      T copy = data;
      return insert(std::move(copy), pth, s...);
    }

    template <typename... S>
    bool remove(const std::string &pth, const S &...s)
    {
      if (_childs.contains(pth))
      {
        if constexpr (sizeof...(s) == 0)
        {
          _childs.erase(pth);
          return true;
        }
        else
        {
          if (_childs[pth].index() == 0)
          {
            return std::get<root>(_childs[pth]).remove(s...);
          }
        }
      }

      return false;
    }

    template <typename T, typename... S>
    std::optional<T> get(const std::string &pth, const S &...)
    {
      return std::nullopt;
    }
  };
}

#include <concepts>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <set>
#include <utility>

namespace stew::dbf::storage::fs::json
{
  template <typename T>
  struct attribute
  {
    std::string_view _label;
    const T &_value;
  };

  template <typename... T>
  struct object
  {
    std::tuple<attribute<T>...> _attrs;
  };

  template <typename T>
  attribute<T> attr(std::string_view l, const T &v)
  {
    return attribute<T>{l, v};
  }

  template <typename... T>
  object<T...> obj(attribute<T> &&...attrs)
  {
    return object<T...>{{attrs...}};
  }

  class ostream
  {
    std::string _s;

  public:
    ~ostream() = default;
    ostream() = default;
    ostream(const ostream &) = default;
    ostream(ostream &&) = default;
    ostream &operator=(const ostream &) = default;
    ostream &operator=(ostream &&) = default;

  public:
    std::string str()
    {
      return _s;
    }

    void append(char c)
    {
      _s.push_back(c);
    }

    void append(std::string_view s)
    {
      _s.append(s.begin(), s.end());
    }
  };

  ostream &operator<<(ostream &sm, char c)
  {
    sm.append(c);
    return sm;
  }

  ostream &operator<<(ostream &sm, std::string_view s)
  {
    sm << '"';
    sm.append(s);
    sm << '"';
    return sm;
  }

  ostream &operator<<(ostream &sm, const std::string &s)
  {
    return sm << static_cast<std::string_view>(s);
  }

  ostream &operator<<(ostream &sm, bool b)
  {
    using namespace std::string_view_literals;

    sm.append(b ? "true"sv : "false"sv);
    return sm;
  }

  template <std::floating_point T>
  ostream &operator<<(ostream &sm, T n)
  {
    sm.append(std::to_string(n));
    return sm;
  }

  template <std::integral T>
  ostream &operator<<(ostream &sm, T n)
  {
    sm.append(std::to_string(n));
    return sm;
  }

  template <typename T>
  ostream &operator<<(ostream &sm, const std::set<T> &l)
  {
    std::size_t i = 0;
    auto b = l.begin();
    auto e = l.end();

    sm << '[';

    while (b != e)
    {
      sm << *b;

      if (i < l.size() - 1)
      {
        sm << ',';
      }

      ++i;
      ++b;
    }

    return sm << ']';
  }

  template <typename T>
  ostream &operator<<(ostream &sm, const std::vector<T> &v)
  {
    sm << '[';

    for (std::size_t i = 0; i < v.size(); ++i)
    {
      sm << v[i];

      if (i < v.size() - 1)
      {
        sm << ',';
      }
    }

    sm << ']';

    return sm;
  }

  template <typename T>
  ostream &operator<<(ostream &sm, const std::list<T> &l)
  {
    std::size_t i = 0;
    auto b = l.begin();
    auto e = l.end();

    sm << '[';

    while (b != e)
    {
      sm << *b;

      if (i < l.size() - 1)
      {
        sm << ',';
      }

      ++i;
      ++b;
    }

    return sm << ']';
  }

  template <typename T>
  ostream &operator<<(ostream &sm, const attribute<T> &a)
  {
    sm.append(a._label);
    return sm << ':' << a._value;
  }

  template <typename... T>
  ostream &operator<<(ostream &sm, const object<T...> &o)
  {
    return std::apply(
        [&sm](const attribute<T> &...attr) -> ostream &
        {
          using namespace std::string_view_literals;

          std::size_t i = 0;
          std::size_t s = sizeof...(T);

          sm << '{';
          (((sm << attr), sm.append(i < s - 1 ? ","sv : ""sv), ++i), ...);
          return sm << '}';
        },
        o._attrs);
  }

  class istream
  {
    std::string_view _s;

  public:
    ~istream() = default;
    istream() = default;
    istream(std::string_view s) : _s(s) {}
    istream(const istream &) = default;
    istream(istream &&) = default;
    istream &operator=(const istream &) = default;
    istream &operator=(istream &&) = default;

  public:
    bool starts_with(char c)
    {
      return _s.starts_with(c);
    }

    bool starts_with(std::string_view s)
    {
      return _s.starts_with(s);
    }

    // void pop_front(std::size_t n = 1)
    // {
    //   if (n <= _s.size())
    //   {
    //     _s.remove_prefix(n);
    //   }
    // }

    char getc()
    {
      char c = _s.front();
      _s.remove_prefix(1);
      return c;
    }
  };

  istream& operator>>(istream& sm, std::string& s)
  {
    if (sm.starts_with('"'))
    {
      sm.getc();
      char c;
      
      while ((c = sm.getc()) != '"')
      {
        s.push_back(c);
      }
    }
  }


}

#include <optional>
#include <string>

namespace stew::dbf::storage::api
{
  template <typename STORAGE>
  class basic_storage
  {
    STORAGE _storage;

  public:
    ~basic_storage() = default;
    basic_storage() = default;
    basic_storage(const std::string &name) : _storage(name) {}
    basic_storage(std::string &&name) : _storage(name) {}
    basic_storage(const basic_storage &) = default;
    basic_storage(basic_storage &&) = default;
    basic_storage &operator=(const basic_storage &) = default;
    basic_storage &operator=(basic_storage &&) = default;

  public:
    template <typename T, typename... S>
    bool insert(const T &data, const std::string &pth, const S &...s)
    {
      return _storage.insert(data, pth, s...);
    }

    template <typename T, typename... S>
    bool insert(T &&data, const std::string &pth, const S &...s)
    {
      return _storage.insert(data, pth, s...);
    }

    template <typename... S>
    bool remove(const std::string &pth, const S &...s)
    {
      return _storage.remove(pth, s...);
    }

    template <typename T, typename... S>
    std::optional<T> get(const std::string &pth, const S &...s)
    {
      return _storage.get(pth, s...);
    }
  };

  using memory = basic_storage<stew::dbf::storage::mem::root>;
  // using fsxml = basic_storage<stew::dbf::storage::fs::xml>;
  // using fsjson = basic_storage<stew::db::storage::fs::json>;

  // template <typename T>
  // using fs = basic_storage<stew::dbf::storage::fs::dir<T>>;
}

#endif