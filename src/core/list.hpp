#ifndef __stew_core_list_hpp__
#define __stew_core_list_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>
#include <core/vector.hpp>

template <typename T>
class list;

template <typename T>
class list_iterator {
 public:
  using type = T;

 private:
  const list<T>& _l;
  size_t _pos;

 public:
  constexpr list_iterator(const list<T>& l) : _l(l) {}

  constexpr bool has() { return _pos != _l.len(); }
  constexpr const type& next() { return _l.at(_pos++); }
};

template <typename T>
class list_oterator {
 public:
  using type = T;

 private:
  list<T>& _l;

 public:
  constexpr list_oterator(list<T>& l) : _l(l) {}
  constexpr bool add(const type& t) { return _l.add(t); }
  constexpr bool add(type&& t) { return _l.add(move(t)); }
};

template <typename T>
constexpr auto iter(const list<T>& l) {
  return list_iterator<T>(l);
}

template <typename T>
constexpr auto oter(const list<T>& l) {
  return list_oterator<T>(l);
}

template <typename T>
class list {
 public:
  using type = T;
  using position = size_t;

 private:
  vector_allocator<T> _alloc;
  vector_allocator<T*> _ialloc;
  size_t _max = 0;
  size_t _len = 0;
  T* _data = nullptr;
  T** _index = nullptr;

 public:
  constexpr ~list() {
    _alloc.destroy(_data);
    _alloc.deallocate(_data);
    _ialloc.deallocate(_index);
    _data = nullptr;
    _index = nullptr;
    _max = 0;
    _len = 0;
  }

  constexpr list() = default;

  constexpr list(const list& l) {
    copy(list_iterator(l), list_oterator(*this));
  }

  constexpr list(list&& l)
      : _data(l._data), _index(l._index), _len(l._len), _max(l._max) {
    l._data = nullptr;
    l._index = nullptr;
    l._len = 0;
    l._max = 0;
  }

  constexpr list& operator=(const list& l) {
    if (this != &l) {
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);
      _ialloc.deallocate(_index);
      _len = 0;
      _max = 0;

      if (l._len != 0) {
        _data = _alloc.allocate(l._len);
        _index = _ialloc.allocate(l._len);
        _max = l._max;
      }

      copy(list_iterator(l), list_oterator(*this));
    }
  }

  constexpr list& operator=(list&& l) {
    if (this != &l) {
      auto td = _data;
      auto id = _index;
      auto tm = _max;
      auto tl = _len;

      _data = l._data;
      _index = l._index;
      _max = l._max;
      _len = l._len;

      l._data = td;
      l._index = id;
      l._max = tm;
      l._len = tl;
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
  constexpr T& at(position p) { return _index[p]; }
  constexpr const T& at(position p) const { return _index[p]; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _ialloc.destroy(_index, _len);
    _len = 0;
  }

  constexpr bool add(const type& t, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (full()) {
      _max = _len * 2 + 10;

      auto itmp = _ialloc.allocate(_max);
      auto dtmp = _alloc.allocate(_max);

      for (size_t i = 0; i < _len; ++i) {
        itmp[i] = _index[i];
        dtmp[i] = move(_data[i]);
      }

      _ialloc.deallocate(_index);
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);

      _index = itmp;
      _data = dtmp;
    }

    if (not full()) {
      for (size_t i = _len; i > p; --p) {
        _index[i] = move(_index[i - 1]);
      }

      _index[p] = &_data[_len];
      _data[_len++] = t;
      return true;
    }

    return false;
  }

  constexpr bool add(type&& t, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (full()) {
      _max = _len * 2 + 10;

      auto itmp = _ialloc.allocate(_max);
      auto dtmp = _alloc.allocate(_max);

      for (size_t i = 0; i < _len; ++i) {
        itmp[i] = _index[i];
        dtmp[i] = move(_data[i]);
      }

      _ialloc.deallocate(_index);
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);

      _index = itmp;
      _data = dtmp;
    }

    if (not full()) {
      for (size_t i = _len; i > p; --p) {
        _index[i] = move(_index[i - 1]);
      }

      _index[p] = _data[_len];
      _data[_len++] = move(t);
      return true;
    }

    return false;
  }

  constexpr bool modify(const T& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _index[p] = t;
      return true;
    }
  }

  constexpr bool modify(T&& t, position p) {
    if (p >= _len) {
      return false;
    } else {
      _index[p] = move(t);
      return true;
    }
  }

  constexpr bool remove(position p) {
    if (p < _len) {
      _index[p].~T();

      for (size_t i = p; i < _len - 1; ++i) {
        _index[i] = _index[i + 1];
      }

      if (p < _len - 1) {
        _data[_index[p]] = move(_data[_len - 1]);
        _index[_len - 2] = _index[p];
      }

      _len -= 1;

      return true;
    }

    return false;
  }
};

#endif
