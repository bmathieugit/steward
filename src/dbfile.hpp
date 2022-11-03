#ifndef __stew_dbfile_hpp__
#define __stew_dbfile_hpp__

#include <filesystem>
#include <string_view>
#include <optional>

// namespace stew::dbf::storage::mem
#include <map>
#include <memory>
#include <optional>
#include <variant>
#include <string_view>

namespace stew::variant
{
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
}

#include <any>
#include <map>
#include <string>
#include <variant>

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
  };
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
    basic_storage(const std::string &name) : _storage(name) {}
    basic_storage(std::string &&name) : _storage(name) {}
    basic_storage(const basic_storage &) = default;
    basic_storage(basic_storage &&) = default;
    basic_storage &operator=(const basic_storage &) = default;
    basic_storage &operator=(basic_storage &&) = default;

  public:
    template <typename T,typename... S>
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
  };

  using memory = basic_storage<stew::dbf::storage::mem::root>;

  // template <typename T>
  // using fs = basic_storage<stew::dbf::storage::fs::dir<T>>;
}

namespace stew::dbf::query
{
  template <typename STORAGE, typename... S>
  std::optional<typename STORAGE::stored_type> select(
      stew::dbf::storage::api::basic_storage<STORAGE> &store,
      const std::string &pth, const S &...s)
  {
    return std::nullopt;
  }

  template <typename STORAGE, typename... S>
  bool insert(
      stew::dbf::storage::api::basic_storage<STORAGE> &store,
      const typename STORAGE::stored_type &data, const std::string &pth, const S &...s)
  {
    return false;
  }

  template <typename STORAGE, typename... S>
  bool insert(
      stew::dbf::storage::api::basic_storage<STORAGE> &store,
      typename STORAGE::stored_type &&data, const std::string &pth, const S &...s)
  {
    return false;
  }

  template <typename STORAGE, typename... S>
  bool update(
      stew::dbf::storage::api::basic_storage<STORAGE> &store,
      const typename STORAGE::stored_type &data, const std::string &pth, const S &...s)
  {
    return insert(store, data, pth, s...);
  }

  template <typename STORAGE, typename... S>
  bool update(
      stew::dbf::storage::api::basic_storage<STORAGE> &store,
      typename STORAGE::stored_type &&data, const std::string &pth, const S &...s)
  {
    return insert(store, data, pth, s...);
  }

  template <typename STORAGE, typename... S>
  bool remove(stew::dbf::storage::api::basic_storage<STORAGE> &store,
              const std::string &pth, const S &...s)
  {
    return false;
  }

  template <typename STORAGE, typename... S>
  std::size_t count(stew::dbf::storage::api::basic_storage<STORAGE> &store,
                    const std::string &pth, const S &...s)
  {
    return 0;
  }

  template <typename STORAGE, typename... S>
  bool exists(stew::dbf::storage::api::basic_storage<STORAGE> &store,
              const std::string &pth, const S &...s)
  {
    return false;
  }

}

#endif