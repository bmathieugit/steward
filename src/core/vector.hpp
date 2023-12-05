#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <core/allocator.hpp>
#include <core/core.hpp>

struct out_of_range {};

template <typename T>
class vector {
 public:
  using type = T;
  using reference = T&;
  using const_reference = const T&;
  using move_reference = T&&;
  using pointer = T*;
  using const_pointer = const T*;

 private:
  allocator<T> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  pointer _data = nullptr;

 public:
  constexpr ~vector() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
  }

  constexpr vector() = default;

  constexpr vector(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(max)) {}

  constexpr vector(const vector& v)
      : _max(v._max), _len(v._len), _data(_alloc.allocate(v._max)) {
    _alloc.copy(_data, v._data, _len);
  }

  constexpr vector(vector&& v) : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  constexpr vector& operator=(const vector& v) {
    if (this != &v) {
      _max = v._max;
      _len = v._len;
      _data = _alloc.allocate(_max);
      _alloc.copy(_data, v._data, _len);
    }

    return *this;
  }

  constexpr vector& operator=(vector&& v) {
    if (this != &v) {
      auto old_max = _max;
      auto old_len = _len;
      auto old_data = _data;

      _max = v._max;
      _len = v._len;
      _data = v._data;

      v._max = old_max;
      v._len = old_len;
      v._data = old_data;
    }

    return *this;
  }

  constexpr auto len() const { return _len; }

  constexpr auto empty() const { return _len == 0; }

  constexpr auto max() const { return _max; }

  constexpr auto full() const { return _len == _max; }

  constexpr bool has(size_t p) const { return p < _len; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;
  }

  constexpr pointer data() { return _data; }

  constexpr const_pointer data() const { return _data; }

  constexpr reference at(size_t p) {
    if (p < _len) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr const_reference at(size_t p) const {
    if (p < _len) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  template <convertible_to<T> U>
  constexpr void add(U&& u, size_t p) {
    if (p > _len) {
      throw out_of_range();
    }

    if (full()) {
      _max = _max * 2 + 10;
      auto dtmp = _alloc.allocate(_max);

      for (size_t i = 0; i < _len; ++i) {
        dtmp[i] = move(_data[i]);
      }

      _data = dtmp;
    }

    if (not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = relay<U>(u);
      ++_len;
    }
  }

  template <convertible_to<T> U>
  constexpr void add(U&& u) {
    add(relay<U>(u), _len);
  }

  constexpr void exchange(size_t p1, size_t p2) {
    if (p1 >= _len or p2 >= _len) {
      throw out_of_range();
    }

    if (p1 != p2) {
      auto tmp1 = move(_data[p1]);
      _data[p1] = move(_data[p2]);
      _data[p2] = move(tmp1);
    }
  }

  template <convertible_to<T> U>
  constexpr void modify(U&& u, size_t p) {
    if (p >= _len) {
      throw out_of_range();
    }

    _data[p] = relay<U>(u);
  }

  constexpr void remove(size_t p) {
    if (p >= _len) {
      throw out_of_range();
    }

    for (size_t i = p; i < _len - 1; ++i) {
      _data[i] = _data[i + 1];
    }

    _data[_len - 1].~T();
    _len -= 1;
  }
};

template <typename V>
concept vector_parameter =
    contains<V, const vector<typename V::type>, vector<typename V::type>>;

template <vector_parameter V1, vector_parameter V2>
constexpr auto operator<=>(V1& v1, V2& v2) {
  return v1.len() <=> v2.len();
}

constexpr auto begin(vector_parameter auto& v) {
  return v.data();
}

constexpr auto end(vector_parameter auto& v) {
  return v.data() + v.len();
}

#endif
