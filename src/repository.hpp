#ifndef __stew_repository_hpp__
#define __stew_repository_hpp__

#include <map>

namespace stew
{
  template <typename K, typename V>
  class repository
  {
    std::map<K, V> data;

  public:
    repository() = default;
    repository(const repository &) = default;
    repository(repository &&) = default;
    repository &operator=(const repository &) = default;
    repository &operator=(repository &&) = default;
    ~repository() = default;
  public:
    V& get(const K& key)
    {
      return data.at(key);
    }

    const V& get(const K& key) const
    {
      return data.at(key);
    }

    void push(const K& key, const V& v) 
    {
      data[key] = v;
    }

    void push(const K& key, V&& v)
    {
      data[key] = std::move(v);
    }

    void erase(const K&key)
    {
      data.erase(key);
    }
  };
}

#endif