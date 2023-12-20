#ifndef __stew_core_map_hpp__
#define __stew_core_map_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/hash.hpp>
#include <core/list.hpp>
#include <core/tuple.hpp>

template <typename K, typename V>
struct entry {
  size_t hash;
  K key;
  V value;
};

template <typename K, typename V>
class map {
 private:
  list<entry<K, V>> _data;

 public:
  constexpr ~map() = default;
  constexpr map() = default;
  constexpr map(size_t len) : _data(len) {}
  constexpr map(const map& m) = default;
  constexpr map(map&& m) = default;
  constexpr map& operator=(const map&) = default;
  constexpr map& operator=(map&&) = default;

 private:
  constexpr size_t to_data_position(const K& k) const {
    auto hash = to_hash<sizeof(size_t) * 8>(k);
    size_t pos = 0;

    auto b = begin(_data);
    auto e = end(_data);

    while (b != e) {
      if ((*(b++)).hash == hash) {
        return pos;
      }
    }

    return max_of<size_t>;
  }

 public:
  constexpr bool has(const K& p) const {
    return to_data_position(p) != max_of<size_t>;
  }

  constexpr bool empty() const { return len() == 0; }
  constexpr size_t len() const { return _data.len(); }

  constexpr const V& at(const K& k) const {
    return _data.at(to_data_position(k)).value;
  }

  constexpr V& at(const K& k) { return _data.at(to_data_position(k)).value; }

 public:
  constexpr void clear() { _data.clear(); }

  constexpr void add(const V& v, const K& k) {
    if (not has(k)) {
      _data.add(entry<K, V>{to_hash<sizeof(size_t) * 8>(k), k, v});
    }
  }

  constexpr void add(V&& v, const K& k) {
    if (not has(k)) {
      _data.add(entry<K, V>{to_hash<sizeof(size_t) * 8>(k), k, move(v)});
    }
  }

  constexpr void modify(const V& v, const K& k) {
    size_t p = to_data_position(k);

    if (p != max_of<decltype(_data.len())>) {
      _data.modify(entry<K, V>{to_hash<sizeof(size_t) * 8>(k), k, v}, p);
    }
  }

  constexpr void modify(V&& v, const K& k) {
    size_t p = to_data_position(k);

    if (p != max_of<decltype(_data.len())>) {
      _data.modify(entry<K, V>{to_hash<sizeof(size_t) * 8>(k), k, move(v)}, p);
    }
  }

  constexpr void remove(const K& k) { _data.remove(to_data_position(k)); }
};

#endif