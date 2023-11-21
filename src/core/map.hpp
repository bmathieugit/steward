#ifndef __stew_core_map_hpp__
#define __stew_core_map_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/hash.hpp>
#include <core/list.hpp>
#include <core/tuple.hpp>

template <typename K, typename V>
class map {
 public:
  using type = V;
  using position = K;

 public:
  list<tuple<size_t, K, V>> _data;

 public:
  constexpr ~map() = default;
  constexpr map() = default;
  constexpr map(size_t len) : _data(len) {}
  constexpr map(const map& m) = default;
  constexpr map(map&& m) = default;
  constexpr map& operator=(const map&) = default;
  constexpr map& operator=(map&&) = default;

 private:
  constexpr size_t to_data_position(const position& p) const {
    auto hash = to_hash<sizeof(size_t) * 8>(p);
    size_t pos = 0;

    auto i = iter(_data);

    while (i.has()) {
      if (get<0>(i.next()) == hash) {
        return pos;
      }

      else {
        ++pos;
      }
    }

    return max_of<size_t>;
  }

 public:
  constexpr bool has(const position& p) const {
    return to_data_position(p) != max_of<size_t>;
  }

  constexpr bool empty() const { return len() == 0; }
  constexpr size_t len() const { return _data.len(); }

  constexpr const type& at(const position& k) const {
    return get<2>(_data.at(to_data_position(k)));
  }

 public:
  constexpr void clear() { _data.clear(); }

  constexpr bool add(const type& v, const position& k) {
    if (not has(k)) {
      _data.add(tuple<size_t, K, V>(to_hash<sizeof(size_t) * 8>(k), k, v));
      return true;
    }

    return false;
  }

  constexpr bool add(type&& v, const position& k) {
    if (not has(k)) {
      _data.add(
          tuple<size_t, K, V>(to_hash<sizeof(size_t) * 8>(k), k, move(v)));
      return true;
    }

    return false;
  }

  // constexpr bool modify(const type& v, const position& k) {
  //   size_t p = to_data_position(k);

  //   if (p != max_of<decltype(_data.len())>) {
  //     _data.modify(map_item<K, V>(k, v), p);
  //     return true;
  //   }

  //   return false;
  // }

  // constexpr bool modify(type&& v, const position& k) {
  //   size_t p = to_data_position(k);

  //   if (p != max_of<decltype(_data.len())>) {
  //     _data.modify(map_item<K, V>(k, move(v)), p);
  //     return true;
  //   }

  //   return false;
  // }

  // constexpr bool remove(const position& p);
};

#endif