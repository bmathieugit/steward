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
    std::pair<std::string_view, const T &> attr(std::string_view l, const T &v)
    {
      return std::pair<std::string_view, const T &>(l, v);
    }

    std::string to_json(const bool &b)
    {
      return b ? "true" : "false";
    }

    std::string to_json(const double &n)
    {
      return std::to_string(n);
    }

    std::string to_json(const std::string &s)
    {
      return '"' + s + '"';
    }

    template <typename T>
    std::string to_json(const std::vector<T> &v)
    {
      std::string s;
      s.push_back('[');

      for (const auto &i : v)
      {
        s.append(to_json(i));
        s.push_back(',');
      }

      s.pop_back();
      s.push_back(']');

      return s;
    }

    template <typename H, typename... T>
    std::string to_json(
        std::pair<std::string_view, const H &> &&head,
        std::pair<std::string_view, const T &> &&...tail)
    {
      std::string s;
      s.push_back('{');
      s.append(head.first);
      s.push_back(':');
      s.append(to_json(head.second));
      ((s.push_back(','), s.append(tail.first), s.push_back(':'), s.append(to_json(tail.second))), ...);
      s.push_back('}');
      return s;
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