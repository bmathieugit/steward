#ifndef __stew_core_map_hpp__
#define __stew_core_map_hpp__

#include <stdio.h>
#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/hash.hpp>
#include <core/tuple.hpp>
#include <core/vector.hpp>

template <typename K, typename V>
struct map_item {
  size_t hash;
  K key;
  V value;

  map_item(const K& k, const V& v) : key(k), value(v) {
    hash = to_hash<sizeof(size_t) * 8>(key);
  }

  map_item(const K& k, V&& v) : key(k), value(move(v)) {
    hash = to_hash<sizeof(size_t) * 8>(key);
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

 private:
  constexpr auto to_data_position(const position& p) const {
    hash = to_hash<sizeof(size_t) * 8>(key);

    size_t pos = max_of<size_t>;

    for (size_t i = 0; i < _data.len(); ++i) {
      if (_data.at(i).hash == hash) {
        if (_data.at(i).key == p) {
          pos = i;
        }
      }
    }

    return pos;
  }

 public:
  constexpr bool has(const position& p) const {
    return to_data_position(p) != max_of<size_t>;
  }

  constexpr bool empty() const { return len() == 0; }
  constexpr size_t len() const { return _data.len(); }

  constexpr const type& at(const position& k) const {
    return _data.at(to_data_position(k)).value;
  }

  constexpr auto ord() const {}

 public:
  constexpr void clear() { _data.clear(); }

  constexpr bool add(const type& v, const position& k) {
    map_item<K, V> mi(k, v);

    if (to_data_position(k) == max_of<size_t>) {
      _data.add(move(mi));
      return true;
    }

    return false;
  }

  constexpr bool add(type&& v, const position& k) {
    map_item<K, V> mi(k, move(v));

    if (to_data_position(k) == max_of<size_t>) {
      _data.add(move(mi));
      return true;
    }

    return false;
  }

  constexpr bool modify(const type& v, const position& k) {
    size_t p = to_data_position(k);

    if (p != max_of<decltype(_data.len())>) {
      _data.modify(map_item<K, V>(k, v), p);
      return true;
    }

    return false;
  }

  constexpr bool modify(type&& v, const position& k) {
    size_t p = to_data_position(k);

    if (p != max_of<decltype(_data.len())>) {
      _data.modify(map_item<K, V>(k, move(v)), p);
      return true;
    }

    return false;
  }

  constexpr bool remove(const position& p);
};

#endif