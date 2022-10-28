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
  struct leaf
  {
    std::string _name;
    T _data;
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
  struct root
  {
    std::string _name;
    std::map<std::string, child<T>> _childs;
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

  template <typename T, typename... S>
  void insert(child<T> &c, T &&data, const std::string &pth, const S &...s)
  {
    if (instance_of<node<T>>(c))
    {
      node<T> &n = std::get<node<T>>(c);

      if (n._childs.contains(pth))
      {
        if constexpr (sizeof...(s) == 0)
        {
          if (instance_of<leaf<T>>(n._childs[pth]))
          {
            std::get<leaf<T>>(n._childs[pth])._data = data;
          }
        }
        else
        {
          insert(n._childs[pth], data, s...);
        }
      }
      else
      {
        if constexpr (sizeof...(s) == 0)
        {
          n._childs[pth] = leaf<T>{._name = pth,
                                   ._data = data};
        }
        else
        {
          n._childs[pth] = node<T>{._name = pth};
          insert(n._childs[pth], data, s...);
        }
      }
    }
  }

  template <typename T, typename... S>
  bool remove(child<T> &c, const std::string &pth, const S &...s)
  {
    if (instance_of<node<T>>(c))
    {
      node<T> &n = std::get<node<T>>(c);

      if (n._childs.contains(pth))
      {
        if constexpr (sizeof...(s) == 0)
        {
          n._childs.erase(pth);
          return true;
        }
        else
        {
          return remove(n._childs[pth], s...);
        }
      }
    }

    return false;
  }

  template <typename T, typename... S>
  void insert(child<T> &c, const T &data, const std::string &pth, S... s)
  {
    T copy = data;
    insert(c, std::move(copy), pth, s...);
  }
}

namespace stew::dbf
{
  namespace fs
  {
    enum class error
    {
      none,
      file_not_found
    };

    template <typename T>
    error select(std::optional<T> &op, std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }

    template <typename T>
    error update(T &t, std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }

    template <typename T>
    error insert(T &t, std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }

    error remove(std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }
  }

  namespace query
  {

  }

}

#endif