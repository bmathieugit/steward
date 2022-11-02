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

#include <list>
#include <string>
#include <variant>
#include <vector>

namespace stew::dbf::storage::format
{
  struct array_tag
  {
  };

  using number = double;
  using string = std::string;
  using boolean = bool;
  using none = std::monostate;

  using data = std::variant<
      none, number, string, boolean>;

  class tree
  {
    std::map<std::string, data> _data;
    std::map<std::string, std::size_t> _arrays;

  public:
    ~tree() = default;
    tree() = default;
    tree(const tree &) = default;
    tree(tree &&) = default;
    tree &operator=(const tree &) = default;
    tree &operator=(tree &&) = default;

  public:
    void size_of(std::string &&pth, std::size_t size)
    {
      _arrays[pth] = size;
    }

    void size_of(const std::string &pth, std::size_t size)
    {
      size_of(std::move(pth), size);
    }

    const std::size_t size_of(const std::string &pth) const
    {
      return _arrays.contains(pth) ? _arrays.at(pth) : 0;
    }

  public:
    template <typename T>
    std::optional<T> get(const std::string &pth) const
    {
      if (_data.contains(pth))
      {
        return std::optional<T>(std::get<T>(_data.at(pth)));
      }

      return std::nullopt;
    }

    template <typename T>
    void set(std::string &&pth, const T &obj)
    {
      _data[pth] = obj;
    }

    template <typename T>
    void set(const std::string &pth, const T &obj)
    {
      set(std::move(pth), obj);
    }
  };

  template <typename T>
  T unmarshall(tree &tr)
  {
    T t;
    unmarshall(tr, t, "");
    return t;
  }

  template <typename T>
  tree marshall(const T &obj)
  {
    tree tr;
    marshall(tr, obj, "");
    return tr;
  }

  template<typename T>
  void marshall(tree& t, const std::vector<T>& v, std::string_view parent_path)
  {
    for (int i=0; i<v.size(); ++i)
    {
      marshall(t, v[i], std::string(parent_path) + "/" + std::to_string(i));
    }
  }

  template<typename T>
  void unmarshall(const tree& t, std::vector<T>& v, std::string_view parent_path)
  {
    auto size = t.size_of(std::string(parent_path));

    for (int i = 0; i < size; ++i)
    {
      T tmp;
      unmarshall(t, tmp, std::string(parent_path) + "/" + std::to_string(i));
      v.push_back(tmp);
    }
  }

  template<typename T>
  void marshall(tree& t, const std::list<T>& l, std::string_view parent_path)
  {
    int i=0;
    for (const auto& item : l)
    {
      marshall(t, l, std::string(parent_path) + "/" + std::to_string(i));
      ++i;
    }
  }
}

namespace stew::dbf::storage::mem
{
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
  class root;

  template <typename T>
  using node = root<T>;

  template <typename T>
  using child = std::variant<leaf<T>, node<T>>;

  template <typename T, typename O>
  constexpr bool instance_of(const child<O> &c)
  {
    return stew::variant::same_index_as<T, child<O>> == c.index();
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
    basic_storage(const std::string &name) : _storage(name) {}
    basic_storage(std::string &&name) : _storage(name) {}
    basic_storage(const basic_storage &) = default;
    basic_storage(basic_storage &&) = default;
    basic_storage &operator=(const basic_storage &) = default;
    basic_storage &operator=(basic_storage &&) = default;

  public:
    template <typename... S>
    bool insert(const STORAGE::stored_type &data, const std::string &pth, const S &...s)
    {
      return _storage.insert(data, pth, s...);
    }

    template <typename... S>
    bool insert(STORAGE::stored_type &&data, const std::string &pth, const S &...s)
    {
      return _storage.insert(data, pth, s...);
    }

    template <typename... S>
    bool remove(const std::string &pth, const S &...s)
    {
      return _storage.remove(pth, s...);
    }
  };

  template <typename T>
  using memory = basic_storage<stew::dbf::storage::mem::root<T>>;

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