#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <core/collection.hpp>
#include <core/utils.hpp>

template <typename T>
struct vector_allocator {
  constexpr T* allocate(size_t n) const {
    return n == 0 ? nullptr : static_cast<T*>(::operator new(n * sizeof(T)));
  }

  constexpr void deallocate(T* p) const noexcept { ::operator delete(p); }

  template <typename... A>
  constexpr void construct(T* p, A&&... args) const {
    new (p) T(relay<A>(args)...);
  }

  constexpr void destroy(T* p) const noexcept {
    if (p != nullptr) {
      p->~T();
    }
  }

  constexpr void destroy(T* p, size_t n) const noexcept {
    if (p != nullptr) {
      for (size_t i = 0; i < n; ++i) {
        p[i].~T();
      }
    }
  }
};

template <typename T>
class vector {
 public:
  using type = T;
  using position = size_t;

 private:
  size_t _max = 0;
  size_t _len = 0;
  T* _data = nullptr;
  vector_allocator<T> _alloc;

 public:
  constexpr ~vector() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
    _data = nullptr;
    _max = 0;
    _len = 0;
  }

  constexpr vector() = default;
  constexpr vector(size_t max)
      : _max(max == 0 ? 10 : max), _len(0), _data(_alloc.allocate(_max)) {}

  constexpr vector(const vector& v) : vector(v._max) { copy(iter(v), *this); }

  template <iterator I>
  constexpr vector(I i)
    requires same_as<rm_cref<typename I::type>, type>
  {
    copy(i, *this);
  }

  constexpr vector& operator=(const vector& v) {
    if (this != &v) {
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._len);
        _max = v._max;
      }

      copy(iter(v), *this);
    }

    return *this;
  }

  constexpr vector(vector&& v) : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
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

 private:
  constexpr size_t increase(size_t more) {
    auto neo = _alloc.allocate(_max + more);

    for (size_t i = 0; i < _len; ++i) {
      neo[i] = move(_data[i]);
    }

    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
    _data = neo;
    return _max = _max + more;
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

  constexpr bool add(const T& t) {
    if (_len == _max) {
      if (increase(_max + 10) == 0) {
        return false;
      }
    }

    _data[_len] = t;
    _len += 1;

    return true;
  }

  constexpr bool add(T&& t) {
    if (_len == _max) {
      if (increase(_max + 10) == 0) {
        return false;
      }
    }

    _data[_len] = move(t);
    _len += 1;

    return true;
  }

  constexpr bool modify(position p, const T& t) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = t;
      return true;
    }
  }

  constexpr bool modify(position p, T&& t) {
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
constexpr auto iter(vector<T>& v) {
  return index_forward_iterator(v);
}

template <typename T>
constexpr auto iter(const vector<T>& v) {
  return index_forward_iterator(v);
}

template <typename T>
class fixed_vector {
 public:
  using type = T;
  using position = size_t;

 private:
  size_t _max = 0;
  size_t _len = 0;
  T* _data = nullptr;
  vector_allocator<T> _alloc;

 public:
  constexpr ~fixed_vector() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
    _data = nullptr;
    _max = 0;
    _len = 0;
  }

  constexpr fixed_vector(size_t max)
      : _max(max == 0 ? 10 : max), _len(0), _data(_alloc.allocate(_max)) {}

  constexpr fixed_vector(const fixed_vector& v) : fixed_vector(v._max) {
    copy(index_forward_iterator(v), *this);
  }

  constexpr fixed_vector& operator=(const fixed_vector& v) {
    if (this != &v) {
      _alloc.destroy(_data, _len);
      _alloc.deallocate(_data);
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._len);
        _max = v._max;
      }

      copy(index_forward_iterator(v), *this);
    }

    return *this;
  }

  constexpr fixed_vector(fixed_vector&& v)
      : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
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

  constexpr bool add(const T& t) {
    if (_len == _max) {
      return false;
    }

    _data[_len] = t;
    _len += 1;

    return true;
  }

  constexpr bool add(T&& t) {
    if (_len == _max) {
      return false;
    }

    _data[_len] = move(t);
    _len += 1;

    return true;
  }

  constexpr bool modify(position p, const T& t) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = t;
      return true;
    }
  }

  constexpr bool modify(position p, T&& t) {
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
constexpr auto iter(fixed_vector<T>& v) {
  return index_forward_iterator(v);
}

template <typename T>
constexpr auto iter(const fixed_vector<T>& v) {
  return index_forward_iterator(v);
}

#endif
