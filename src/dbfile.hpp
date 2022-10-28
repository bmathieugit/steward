#ifndef __stew_dbfile_hpp__
#define __stew_dbfile_hpp__

#include <filesystem>
#include <string_view>
#include <optional>

// namespace stew::dbf::storage::mem
#include <map>
#include <memory>
#include <variant>
#include <string_view>

namespace stew::dbf::storage::mem
{
  // Ici il faut mettre en place un tree pour permettre de faire
  // un stockage en mémoire d'une structure arborecente
  // Chaque noeud aura un label qui donnera son nom dans l'arborecence

  template <typename T, typename V, std::size_t i = 0>
  constexpr std::size_t same_index_as_impl()
  {
    static_assert(std::variant_size_v<V> > i, "Type not found in variant");

    if constexpr (i == std::variant_size_v<V>)
    {
      return i;
    }
    else if constexpr (std::is_same_v<std::variant_alternative_t<i, V>, T>)
    {
      return i;
    }
    else
    {
      return same_index_as_impl<T, V, i + 1>();
    }
  }

  template <typename T, typename V>
  constexpr int same_index_as = same_index_as_impl<T, V>();

  template <typename T>
  class leaf
  {
    std::string _name;
    T _data;

  public:
    using stored_type = T;

  public:
    ~leaf() = default;
    leaf() = default;
    leaf(const std::string &name, const T &data)
        : _name(name), _data(data) {}
    leaf(const std::string &name, T &&data)
        : _name(name), _data(data) {}
    leaf(std::string &&name, const T &data)
        : _name(name), _data(data) {}
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

    const T &data() const
    {
      return _data;
    }

    T &data()
    {
      return _data;
    }

    void data(const T &d)
    {
      _data = d;
    }

    void data(T &&d)
    {
      _data = d;
    }
  };

  template <typename T>
  struct root;

  template <typename T>
  using node = root<T>;

  template <typename T>
  using child = std::variant<leaf<T>, node<T>>;

  template <typename T, typename O>
  constexpr bool instance_of(const child<O> &c)
  {
    return same_index_as<T, child<O>> == c.index();
  }

  template <typename T>
  class root
  {
    std::string _name;
    std::map<std::string, child<T>> _childs;

  public:
    using stored_type = T;

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
    template <typename... S>
    bool insert(T &&data, const std::string &pth, const S &...s)
    {
      if (_childs.contains(pth))
      {
        if constexpr (sizeof...(s) == 0)
        {
          if (instance_of<leaf<T>>(_childs[pth]))
          {
            std::get<leaf<T>>(_childs[pth]).data(data);
            return true;
          }
        }
        else
        {
          return std::get<node<T>>(_childs[pth]).insert(data, s...);
        }
      }
      else
      {
        if constexpr (sizeof...(s) == 0)
        {
          _childs[pth] = leaf<T>(pth, data);
          return true;
        }
        else
        {
          _childs[pth] = node<T>(pth);
          return std::get<node<T>>(_childs[pth]).insert(data, s...);
        }
      }

      return false;
    }

    template <typename... S>
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
          if (instance_of<root<T>>(_childs[pth]))
          {
            return std::get<root<T>>(_childs[pth]).remove(s...);
          }
        }
      }

      return false;
    }
  };

  template <typename T>
  root<T> make_root(const std::string &name)
  {
    return root<T>{name};
  }

  template <typename T>
  root<T> make_root(std::string &&name)
  {
    return root<T>{std::move(name)};
  }
}

namespace stew::dbf::storage::api
{
  template <typename STORAGE>
  class basic_storage
  {
    STORAGE _storage;

  public:
    ~basic_storage() = default;
    basic_storage() = default;
    basic_storage(const basic_storage &) = default;
    basic_storage(basic_storage &&) = default;
    basic_storage &operator=(const basic_storage &) = default;
    basic_storage &operator=(basic_storage &&) = default;

  public:
    template <typename... S>
    bool insert(const STORAGE::stored_type &data, const std::string &pth, const S &...s)
    {
      return true;
      // return insert(_storage
    }
  };

  template <typename T>
  using memory = basic_storage<stew::dbf::storage::mem::root<T>>;

}

#endif