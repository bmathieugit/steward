#ifndef __stew_core_map_hpp__
#define __stew_core_map_hpp__

#include <stdio.h>
#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/fnv-ostream.hpp>
#include <core/tuple.hpp>
#include <core/vector.hpp>

template <typename K, typename V>
struct map_item {
  size_t hash;
  K key;
  V value;

  map_item(const K& k, const V& v) : key(k), value(v) {
    auto fnv = fnv_ostream<sizeof(size_t) * 8>{};
    hash = (fnv << key).hash();
  }
};

template <typename K, typename V>
class map {
 public:
  using type = V;
  using position = K;

 private:
  vector<map_item<K, V>> _data;

 public:
  constexpr ~map() = default;
  constexpr map() = default;
  constexpr map(size_t len) : _data(len) {}
  constexpr map(const map& m) = default;
  constexpr map(map&& m) = default;
  constexpr map& operator=(const map&) = default;
  constexpr map& operator=(map&&) = default;

 public:
  constexpr bool has(const position& p) const {
    auto fnv = fnv_ostream<sizeof(size_t) * 8>{};
    auto hash = (fnv << p).hash();
    return find(_data,
                [hash, &p](const map_item<K, V>& item) {
                  return item.hash == hash and item.key == p;
                })
        .has();
  }

  constexpr bool empty() const { return len() == 0; }
  constexpr size_t len() const { return _data.len(); }

  constexpr const type& at(const position& p) const {
    auto fnv = fnv_ostream<sizeof(size_t) * 8>{};
    auto hash = (fnv << p).hash();
    return _data.at(find(_data, [hash, &p](const map_item<K, V>& item) {
                      return hash == item.hash and p == item.key;
                    }).get());
  }

  constexpr auto ord() const {}

 public:
  constexpr void clear() { _data.clear(); }

  constexpr bool add(const V& v, const K& k) {
    map_item<K, V> mi(k, v);

    if (not find(_data, [&mi](const map_item<K, V>& item) {
              return mi.hash == item.hash and mi.key == item.key;
            }).has()) {
      _data.add(move(mi));
      return true;
    }

    return false;
  }

  // constexpr bool add(const V& v, const K& k) {
  //   map_item<K, V> mi(move(k), move(v));

  //   if (not find(_data, [&mi](const map_item<K, V>& item) {
  //             return mi.hash == item.hash and mi.key == item.key;
  //           }).has()) {
  //     _data.add(move(mi));
  //     return true;
  //   }

  //   return false;
  // }

  constexpr bool modify(const V& v, const K& k) {
    map_item<K, V> mi(k, v);

    auto found = find(_data, [&mi](const map_item<K, V>& item) {
      return mi.hash == item.hash and mi.key == item.key;
    });

    if (found.has()) {
      _data.modify(found.get(), move(mi));
      return true;
    }

    return false;
  }

  constexpr bool remove(const position& p);
};

#endif