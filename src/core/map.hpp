#ifndef __stew_core_map_hpp__
#define __stew_core_map_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/hash.hpp>
#include <core/tuple.hpp>
#include <core/vector.hpp>

template <typename K, typename V>
struct map_item {
  K key;
  V value;

  constexpr map_item(const K& k, const V& v) : key(k), value(v) {}
  constexpr map_item(const K& k, V&& v) : key(k), value(move(v)) {}
};

template <typename K>
struct map_index {
  size_t hash;
  size_t rank;
  size_t index;

  constexpr map_index(const K& key, size_t i)
      : hash(to_hash<sizeof(size_t) * 8>(key)), index(i) {}
};

template <typename K, typename V>
class map {
 public:
  using type = V;
  using position = K;

 private:
  vector<map_index<K>> _index;
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
    auto hash = to_hash<sizeof(size_t) * 8>(p);

    size_t pos = max_of<size_t>;

    int left = 0;
    int right = _index.len() - 1;

    while (left <= right) {
      int middle = left + (right - left) / 2;

      if (_index.at(middle).hash == hash) {
        pos = middle;
        break;
      }

      else if (_index.at(middle).hash > hash) {
        right = middle - 1;
      } 
      
      else {
        left = middle + 1;
      }
    }

    printf("pos %lu\n", pos);


    return _data.at(pos).key == p ? _index.at(pos).index : max_of<size_t>;
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
    if (to_data_position(k) == max_of<size_t>) {
      _data.add(map_item<K, V>(k, v));
      _index.add(map_index<K>(k, _data.len() - 1));
      return true;
    }

    return false;
  }

  constexpr bool add(type&& v, const position& k) {
    if (to_data_position(k) == max_of<size_t>) {
      _data.add(map_item<K, V>(k, move(v)));
      _index.add(map_index<K>(k, _data.len() - 1));
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