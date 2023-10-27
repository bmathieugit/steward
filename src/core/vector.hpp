#ifndef __n_vector_hpp__
#define __n_vector_hpp__

#include <core/algorithm.hpp>
#include <core/core.hpp>

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
  vector_allocator<T> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  T* _data = nullptr;

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

  constexpr vector(const vector& v) : vector(v._max) { copy(v, *this); }

  template <input_stream I>
  constexpr vector(I i)
    requires same_type<vector, I>
  {
    copy(i, index_forward_output_stream(*this));
  }

  template <collection C>
  constexpr vector(const C& c)
    requires same_type<vector, C>
      : vector(c.len()) {
    copy(c, *this);
  }

  constexpr vector(vector&& v) : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
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

      copy(v, *this);
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

  template <input_stream I>
  constexpr vector& operator=(I i)
    requires same_type<vector, I>
  {
    clear();
    copy(i, index_forward_output_stream(*this));
  }

  template <collection C>
  constexpr vector& operator=(const C& c)
    requires same_type<vector, C>
  {
    clear();
    copy(c, *this);
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
  constexpr auto data() { return _data; }
  constexpr auto data() const { return _data; }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }
  constexpr auto ord() const { return index_based_ordinal<position>(0, _len); }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr T& at(position p) { return _data[p]; }
  constexpr const T& at(position p) const { return _data[p]; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;
  }

  constexpr bool add(const type& t, position p = max_of<decltype(_len)>) {
    p = p == max_of<decltype(_len)> ? _len : p;

    if (_len == _max) {
      if (increase(_max + 10) == 0) {
        return false;
      }
    }

    for (size_t i = _len; i > p; --i) {
      _data[i] = move(_data[i - 1]);
    }

    _data[p] = t;
    ++_len;
    return true;
  }

  constexpr bool add(type&& t, position p = max_of<decltype(_len)>) {
    p = p == max_of<decltype(_len)> ? _len : p;

    if (_len == _max) {
      if (increase(_max + 10) == 0) {
        return false;
      }
    }

    for (size_t i = _len; i > p; --i) {
      _data[i] = move(_data[i - 1]);
    }

    _data[p] = move(t);
    ++_len;
    return true;
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
constexpr auto istream(const vector<T>& v) {
  return index_forward_input_stream(v);
}

template <typename T>
constexpr auto ristream(const vector<T>& a) {
  return index_backward_input_stream(a);
}

template <typename T>
constexpr auto ostream(vector<T>& v) {
  return index_forward_output_stream(v);
}

template <typename T>
class fixed_vector {
 public:
  using type = T;
  using position = size_t;

 private:
  vector_allocator<T> _alloc;
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

  constexpr fixed_vector(size_t max)
      : _max(max == 0 ? 10 : max), _len(0), _data(_alloc.allocate(_max)) {}

  constexpr fixed_vector(const fixed_vector& v) : fixed_vector(v._max) {
    copy(v, *this);
  }

  template <collection C>
  constexpr fixed_vector(const C& c)
    requires same_type<fixed_vector, C>
      : fixed_vector(c.len()) {
    copy(c, *this);
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
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._len);
        _max = v._max;
      }

      copy(v, *this);
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

  template <collection C>
  constexpr fixed_vector& operator=(const C& c)
    requires same_type<fixed_vector, C>
  {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
    _len = 0;
    _max = 0;

    if (c.len() != 0) {
      _data = _alloc.allocate(c.len());
      _max = c.len();
    }

    copy(c, *this);

    return *this;
  }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }
  constexpr auto ord() const { return index_based_ordinal<position>(0, _len); }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr T& at(position p) { return _data[p]; }
  constexpr const T& at(position p) const { return _data[p]; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;
  }

  constexpr bool add(const type& t, position p = max_of<decltype(_len)>) {
    p = p == max_of<decltype(_len)> ? _len : p;

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
    p = p == max_of<decltype(_len)> ? _len : p;

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
constexpr auto istream(const fixed_vector<T>& v) {
  return index_forward_input_stream(v);
}

template <typename T>
constexpr auto ristream(const fixed_vector<T>& a) {
  return index_backward_input_stream(a);
}

template <typename T>
constexpr auto ostream(fixed_vector<T>& v) {
  return index_forward_output_stream(v);
}

#endif
