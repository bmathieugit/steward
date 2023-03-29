#ifndef __steward_vector_hpp__
#define __steward_vector_hpp__

#include <stew/algorithm.hpp>
#include <stew/array.hpp>
#include <stew/iterator.hpp>
#include <stew/maybe.hpp>
#include <stew/meta.hpp>
#include <stew/smarts.hpp>

namespace stew {
template <typename T, size_t N>
class static_vector {
 private:
  array<T, N> _data;
  size_t _size = 0;

 public:
  constexpr ~static_vector() = default;
  constexpr static_vector() = default;

  template <input_range R>
  constexpr static_vector(R &&r) {
    push(relay<R>(r));
  }

  constexpr static_vector(const static_vector &) = default;
  constexpr static_vector(static_vector &&) = default;
  constexpr static_vector &operator=(const static_vector &) = default;
  constexpr static_vector &operator=(static_vector &&) = default;

  template <input_range R>
  constexpr static_vector &operator=(R &&r) {
    return (*this = static_vector(relay<R>(r)));
  }

 public:
  constexpr auto size() const { return _size; }

  constexpr auto empty() const { return _size == 0; }

  constexpr auto full() const { return _size == N; }

 public:
  constexpr auto begin() { return _data.begin(); }

  constexpr auto end() { return _data.begin() + _size; }

  constexpr const auto begin() const { return _data.begin(); }

  constexpr const auto end() const { return _data.begin() + _size; }

  constexpr T &operator[](size_t i) {
    assert(i < N);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < N);
    return _data[i];
  }

 public:
  template <convertible_to<T> U>
  constexpr void push(U &&u) {
    if (!full()) {
      _data[_size] = relay<U>(u);
      ++_size;
    }
  }

  template <input_range R>
  constexpr void push(R &&r) {
    copy(relay<R>(r), pushing<T>(*this));
  }

  constexpr maybe<T> pop() {
    if (_size != 0) {
      return maybe<T>(transfer(_data[_size--]));
    } else {
      return maybe<T>();
    }
  }

  constexpr void clear() { _size = 0; }
};

template <typename T>
class fixed_vector {
 private:
  size_t _size{0};
  size_t _max{0};
  owning<T[]> _data;

 public:
  constexpr ~fixed_vector() = default;
  constexpr fixed_vector() = default;

  constexpr explicit fixed_vector(size_t max)
      : _size{0}, _max{max}, _data{new T[_max + 1]{}} {}

  constexpr fixed_vector(const fixed_vector &o) : fixed_vector(o._max) {
    push(o);
  }

  constexpr fixed_vector(fixed_vector &&o)
      : _size{transfer(o._size)},
        _max{transfer(o._max)},
        _data{transfer(o._data)} {}

  template <input_range R>
  constexpr fixed_vector(R &&r)
    requires distanciable_iterator<decltype(stew::begin(r))>
      : fixed_vector(stew::end(relay<R>(r)) - stew::begin(relay<R>(r))) {
    push(relay<R>(r));
  }

  constexpr fixed_vector &operator=(fixed_vector o) {
    _size = transfer(o._size);
    _max = transfer(o._max);
    _data = transfer(o._data);
    return *this;
  }

  template <input_range R>
  constexpr fixed_vector &operator=(R &&r) {
    return (*this = fixed_vector(relay<R>(r)));
  }

 public:
  constexpr auto begin() { return _data.get(); }

  constexpr auto end() { return begin() + _size; }

  constexpr const auto begin() const { return _data.get(); }

  constexpr const auto end() const { return begin() + _size; }

  constexpr T &operator[](size_t i) {
    assert(i < _size);
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < _size);
    return _data[i];
  }

 public:
  constexpr bool empty() const { return _size == 0; }
  constexpr bool full() const { return _size == _max; }
  constexpr auto size() const { return _size; }

 public:
  constexpr void push(const T &t) {
    if (!full()) {
      _data[_size] = t;
      _size += 1;
    }
  }

  constexpr void push(T &&t) {
    if (!full()) {
      _data[_size] = transfer(t);
      _size += 1;
    }
  }

  template <input_range R>
  constexpr void push(R &&r) {
    copy(relay<R>(r), pushing<T>(*this));
  }

  constexpr maybe<T> pop() {
    if (_size != 0) {
      return maybe<T>(transfer(_data[_size--]));
    } else {
      return maybe<T>();
    }
  }

  constexpr void clear() {
    _size = 0;

    if (_max > 0) {
      _data[0] = T();
    }
  }
};

template <typename T>
class vector {
 private:
  fixed_vector<T> _data;

 public:
  constexpr ~vector() = default;
  constexpr vector() = default;
  constexpr explicit vector(size_t max) : _data(max) {}

  template <input_range R>
  constexpr vector(R &&r)
      : vector(stew::end(relay<R>(r)) - stew::begin(relay<R>(r))) {
    push(relay<R>(r));
  }

  constexpr vector(const vector &) = default;
  constexpr vector(vector &) = default;
  constexpr vector &operator=(const vector &) = default;
  constexpr vector &operator=(vector &&) = default;

  template <input_range R>
  constexpr vector &operator=(R &&r) {
    return (*this = vector(relay<R>(r)));
  }

 public:
  constexpr auto begin() { return stew::begin(_data); }

  constexpr auto end() { return stew::end(_data); }

  constexpr const auto begin() const { return stew::begin(_data); }

  constexpr const auto end() const { return stew::end(_data); }

  constexpr T &operator[](size_t i) {
    assert(i < size());
    return _data[i];
  }

  constexpr const T &operator[](size_t i) const {
    assert(i < size());
    return _data[i];
  }

 public:
  constexpr auto size() const { return _data.size(); }

  constexpr auto empty() const { return _data.empty(); }

  constexpr auto full() const { return _data.full(); }

 public:
  constexpr void push(const T &t) {
    if (_data.full()) {
      fixed_vector<T> tmp = transfer(_data);
      _data = fixed_vector<T>(tmp.size() * 2 + 10);
      _data.push(transfer(tmp));
    }

    _data.push(t);
  }

  constexpr void push(T &&t) {
    if (_data.full()) {
      fixed_vector<T> tmp = transfer(_data);
      _data = fixed_vector<T>(tmp.size() * 2 + 10);
      _data.push(transfer(tmp));
    }

    _data.push(transfer(t));
  }

  template <input_range R>
  constexpr void push(R &&r)
    requires not_convertible_to<R, T>
  {
    copy(relay<R>(r), pushing<T>(*this));
  }

  constexpr auto pop() { return _data.pop(); }

  constexpr auto clear() { return _data.clear(); }
};

}  // namespace stew

#endif
