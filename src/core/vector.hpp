#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <core/algorithm.hpp>
#include <core/allocator.hpp>
#include <core/core.hpp>

template <typename T, size_t N>
class static_vector {
 public:
  using type = T;
  using position = size_t;

 private:
  T _data[N];
  size_t _len = 0;

 public:
  constexpr ~static_vector() = default;
  constexpr static_vector() = default;
  constexpr static_vector(const T (&data)[N]) {
    for (size_t i = 0; i < N; ++i) {
      add(data[i]);
    }
  }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return N; }
  constexpr auto full() const { return _len == N; }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr T& at(position p) { return _data[p]; }
  constexpr const T& at(position p) const { return _data[p]; }

  constexpr void clear() {
    for (size_t i = 0; i < _len; ++i) {
      _data[i].~T();
    }

    _len = 0;
  }

  constexpr bool add(const type& t, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (p <= _len and not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = t;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool add(type&& t, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (p <= _len and not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = move(t);
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool exchange(position p1, position p2) {
    if (has(p1) and has(p2)) {
      auto tmp1 = move(_data[p1]);
      _data[p1] = move(_data[p2]);
      _data[p2] = move(tmp1);

      return true;
    }

    else {
      return false;
    }
  }

  constexpr bool modify(const type& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = t;
      return true;
    }
  }

  constexpr bool modify(type&& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = move(t);
      return true;
    }
  }

  constexpr bool remove(position p) {
    if (p >= _len) {
      return false;
    } else {
      for (size_t i = p; i < _len - 1; ++i) {
        _data[i] = move(_data[i + 1]);
      }

      _data[_len - 1] = T();
      _len -= 1;

      return true;
    }
  }
};

template <typename T, size_t N>
constexpr auto iter(const static_vector<T, N>& a) {
  return index_based_iterator(a);
}

template <typename T, size_t N>
constexpr auto riter(const static_vector<T, N>& a) {
  return index_based_riterator(a);
}

template <typename T, size_t N>
constexpr auto oter(static_vector<T, N>& a) {
  return index_based_oterator(a);
};

template <typename T>
class fixed_vector {
 public:
  using type = T;
  using position = size_t;

 private:
  allocator<T> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  T* _data = nullptr;

 public:
  constexpr ~fixed_vector() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
    _data = nullptr;
    _max = 0;
    _len = 0;
  }

  constexpr fixed_vector() = default;

  constexpr fixed_vector(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(_max)) {}

  constexpr fixed_vector(const fixed_vector& v) : fixed_vector(v._max) {
    copy(index_based_iterator(v), index_based_oterator(*this));
  }

  constexpr fixed_vector(fixed_vector&& v)
      : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  constexpr fixed_vector& operator=(const fixed_vector& v) {
    if (this != &v) {
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);
      _data = nullptr;
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._len);
        _max = v._max;
      }

      copy(index_based_iterator(v), index_based_oterator(*this));
    }

    return *this;
  }

  constexpr fixed_vector& operator=(fixed_vector&& v) {
    if (this != &v) {
      auto td = _data;
      auto tm = _max;
      auto tl = _len;

      _data = v._data;
      _max = v._max;
      _len = v._len;

      v._data = td;
      v._max = tm;
      v._len = tl;
    }

    return *this;
  }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr T& at(position p) { return _data[p]; }
  constexpr const T& at(position p) const { return _data[p]; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;
  }

  constexpr bool add(const type& t, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (p <= _len and not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = t;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool add(type&& t, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (p <= _len and not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = t;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool exchange(position p1, position p2) {
    if (has(p1) and has(p2)) {
      auto tmp1 = move(_data[p1]);
      _data[p1] = move(_data[p2]);
      _data[p2] = move(tmp1);
      return true;
    }

    else {
      return false;
    }
  }

  constexpr bool modify(const T& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = t;
      return true;
    }
  }

  constexpr bool modify(T&& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = move(t);
      return true;
    }
  }

  constexpr bool remove(position p) {
    if (p >= _len) {
      return false;
    } else {
      for (size_t i = p; i < _len - 1; ++i) {
        _data[i] = _data[i + 1];
      }

      _data[_len - 1].~T();
      _len -= 1;

      return true;
    }
  }
};

template <typename T>
constexpr auto iter(const fixed_vector<T>& v) {
  return index_based_iterator(v);
}

template <typename T>
constexpr auto riter(const fixed_vector<T>& v) {
  return index_based_riterator(v);
}

template <typename T>
constexpr auto oter(fixed_vector<T>& v) {
  return index_based_oterator(v);
}

template <typename T>
class vector {
 public:
  using type = T;
  using position = size_t;

 private:
  allocator<T> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  T* _data = nullptr;

 public:
  constexpr ~vector() = default;
  constexpr vector() = default;
  constexpr vector(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(max)) {}

  constexpr vector(const vector& v) : vector(v._max) {
    copy(index_based_iterator(v), index_based_oterator(*this));
  }

  constexpr vector(vector&& v) : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  template <iterator I>
  constexpr vector(I i) : vector() {
    copy(i, index_based_oterator(*this));
  }

  constexpr vector& operator=(const vector& v) {
    if (this != &v) {
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);
      _data = nullptr;
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._len);
        _max = v._max;
      }

      copy(index_based_iterator(v), index_based_oterator(*this));
    }

    return *this;
  }

  constexpr vector& operator=(vector&& v) {
    if (this != &v) {
      auto td = _data;
      auto tm = _max;
      auto tl = _len;

      _data = v._data;
      _max = v._max;
      _len = v._len;

      v._data = td;
      v._max = tm;
      v._len = tl;
    }

    return *this;
  }

  template <iterator I>
  constexpr vector& operator=(I i) {
    clear();
    copy(i, index_based_oterator(*this));
    return *this;
  }

 public:
  constexpr auto data() { return _data; }
  constexpr auto data() const { return _data; }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr T& at(position p) { return _data[p]; }
  constexpr const T& at(position p) const { return _data[p]; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;
  }

  constexpr bool add(const type& t, position p = max_of<size_t>) {
    p = p >= max_of<size_t> ? _len : p;

    if (full()) {
      _max = _max * 2 + 10;
      auto dtmp = _alloc.allocate(_max);

      for (size_t i = 0; i < _len; ++i) {
        dtmp[i] = move(_data[i]);
      }

      _data = move(dtmp);
    }

    if (not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = t;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool add(type&& t, position p = max_of<size_t>) {
    p = p >= max_of<size_t> ? _len : p;

    if (full()) {
      _max = _max * 2 + 10;
      auto dtmp = _alloc.allocate(_max);

      for (size_t i = 0; i < _len; ++i) {
        dtmp[i] = move(_data[i]);
      }

      _data = move(dtmp);
    }

    if (not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = move(t);
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool exchange(position p1, position p2) {
    if (has(p1) and has(p2)) {
      auto tmp1 = move(_data[p1]);
      _data[p1] = move(_data[p2]);
      _data[p2] = move(tmp1);
      return true;
    }

    else {
      return false;
    }
  }

  constexpr bool modify(const T& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = t;
      return true;
    }
  }

  constexpr bool modify(T&& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = move(t);
      return true;
    }
  }

  constexpr bool remove(position p) {
    if (p >= _len) {
      return false;
    } else {
      for (size_t i = p; i < _len - 1; ++i) {
        _data[i] = _data[i + 1];
      }

      _data[_len - 1].~T();
      _len -= 1;

      return true;
    }
  }
};

template <typename T>
constexpr auto iter(const vector<T>& v) {
  return index_based_iterator(v);
}

template <typename T>
constexpr auto riter(const vector<T>& v) {
  return index_based_riterator(v);
}

template <typename T>
constexpr auto oter(vector<T>& v) {
  return index_based_oterator(v);
}

#endif
