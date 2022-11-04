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
#include <utility>

namespace stew::dbf::storage::fs
{
  namespace json
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

    class stream
    {
      std::string _s;

    public:
      ~stream() = default;
      stream() = default;
      stream(const stream &) = default;
      stream(stream &&) = default;
      stream &operator=(const stream &) = default;
      stream &operator=(stream &&) = default;

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

    stream &operator<<(stream &sm, char c)
    {
      sm.append(c);
      return sm;
    }

    stream &operator<<(stream &sm, std::string_view s)
    {
      sm << '"';
      sm.append(s);
      sm << '"';
      return sm;
    }

    stream &operator<<(stream &sm, const std::string &s)
    {
      return sm << static_cast<std::string_view>(s);
    }

    stream &operator<<(stream &sm, bool b)
    {
      using namespace std::string_view_literals;

      sm.append(b ? "true"sv : "false"sv);
      return sm;
    }

    template <std::floating_point T>
    stream &operator<<(stream &sm, T n)
    {
      sm.append(std::to_string(n));
      return sm;
    }

    template <std::integral T>
    stream &operator<<(stream &sm, T n)
    {
      sm.append(std::to_string(n));
      return sm;
    }

    template <typename T>
    stream &operator<<(stream &sm, const std::vector<T> &v)
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
    stream &operator<<(stream &sm, const attribute<T> &a)
    {
      sm.append(a._label);
      return sm << ':' << a._value;
    }

    template <typename... T>
    stream &operator<<(stream &sm, const object<T...> &o)
    {
      return std::apply(
          [&sm](const attribute<T> &...attr) -> stream &
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