#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <core/allocator.hpp>
#include <core/core.hpp>
#include <core/result.hpp>
#include <core/span.hpp>

template <character C>
class basic_string_view;

template <character C>
class basic_string {
 private:
  allocator<C> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  C* _data = nullptr;

 public:
  constexpr ~basic_string() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data);
  }

  constexpr basic_string() = default;

  constexpr basic_string(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(max + 1)) {
    _data[_len] = 0;
  }
  template <iterator I>
    requires distanciable<I>
  constexpr basic_string(I it) : basic_string(it.distance()) {
    while (it.has_next()) {
      this->add(it.next());
    }
  }

  template <iterator I>
    requires(not distanciable<I>)
  constexpr basic_string(I it) {
    while (it.has_next()) {
      this->add(it.next());
    }
  }

  constexpr basic_string(basic_string_view<C> s) : basic_string(iter(s)) {}

  constexpr basic_string(const basic_string& v)
      : _max(v._max), _len(v._len), _data(_alloc.allocate(v._max + 1)) {
    _alloc.copy(_data, v._data, _len);
    _data[_len] = 0;
  }

  constexpr basic_string(basic_string&& v)
      : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  constexpr basic_string& operator=(const basic_string& v) {
    if (this != &v) {
      _max = v._max;
      _len = v._len;
      _data = _alloc.allocate(_max + 1);
      _alloc.copy(_data, v._data, _len);
      _data[_len] = 0;
    }

    return *this;
  }

  constexpr basic_string& operator=(basic_string&& v) {
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

  constexpr C* data() { return _data; }

  constexpr const C* data() const { return _data; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;

    if (_data != nullptr) {
      _data[0] = 0;
    }
  }

  constexpr C& at(size_t p) {
    if (p < _len) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  constexpr const C& at(size_t p) const {
    if (p < _len) {
      return _data[p];
    } else {
      throw out_of_range();
    }
  }

  template <convertible_to<C> U>
  constexpr void add(U&& u, size_t p) {
    if (p > _len) {
      throw out_of_range();
    }

    if (full()) {
      auto nmax = _max * 2 + 10;
      auto dtmp = _alloc.allocate(nmax);

      for (size_t i = 0; i < _len; ++i) {
        dtmp[i] = move(_data[i]);
      }

      _alloc.deallocate(_data);
      _max = nmax;
      _data = dtmp;
    }

    if (not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = move(_data[i - 1]);
      }

      _data[p] = relay<U>(u);
      ++_len;
      _data[_len] = 0;
    }
  }

  template <convertible_to<C> U>
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

  template <convertible_to<C> U>
  constexpr void modify(U&& u, size_t p) {
    if (p >= _len) {
      throw out_of_range();
    }

    C c = relay<U>(u);

    _data[p] = c;

    if (c == 0) {
      _len = p;
    }
  }

  constexpr void remove(size_t p) {
    if (p >= _len) {
      throw out_of_range();
    }

    for (size_t i = p; i < _len - 1; ++i) {
      _data[i] = _data[i + 1];
    }

    _len -= 1;
    _data[_len] = 0;
  }
};

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <character C>
class basic_string_oterator {
 public:
  using type = C;

 private:
  basic_string<C>& _str;

 public:
  constexpr basic_string_oterator(basic_string<C>& s) : _str(s) {}

 public:
  constexpr void sext(const C& c) { _str.add(c); }
  constexpr void sext(C&& c) { _str.add(move(c)); }
};

template <character C>
constexpr auto oter(basic_string<C>& str) {
  return basic_string_oterator<C>(str);
}

template <character C>
class basic_string_iterator {
 public:
  using type = C;

 private:
  C* _begin;
  C* _sentinel;

 public:
  constexpr basic_string_iterator(basic_string<C>& a)
      : _begin(a.data()), _sentinel(a.data() + a.len()) {}

 public:
  constexpr bool has_next() const { return _begin != _sentinel; }

  constexpr C& next() { return *(_begin++); }

  constexpr size_t distance() const { return _sentinel - _begin; }
};

template <character C>
constexpr auto iter(basic_string<C>& a) {
  return basic_string_iterator(a);
}

template <character C>
class basic_string_const_iterator {
 public:
  using type = const C;

 private:
  const C* _begin;
  const C* _sentinel;

 public:
  constexpr basic_string_const_iterator(const basic_string<C>& a)
      : _begin(a.data()), _sentinel(a.data() + a.len()) {}

 public:
  constexpr bool has_next() const { return _begin != _sentinel; }

  constexpr const C& next() { return *(_begin++); }

  constexpr size_t distance() const { return _sentinel - _begin; }
};

template <character C>
constexpr auto iter(const basic_string<C>& a) {
  return basic_string_const_iterator(a);
}

template <character C>
class basic_string_view : public span<const C> {
 public:
  constexpr basic_string_view() = default;
  constexpr basic_string_view(const C* begin, const C* end)
      : span<const C>(begin, end) {}

  constexpr basic_string_view(const C* begin, size_t len)
      : span<const C>(begin, len) {}

  constexpr basic_string_view(const C* begin)
      : span<const C>(begin, strlen(begin)) {}

  constexpr basic_string_view(const basic_string<C>& s)
      : span<const C>(begin(s), s.len()) {}

  constexpr basic_string_view(const basic_string_view&) = default;

  constexpr basic_string_view(basic_string_view&&) = default;

  constexpr basic_string_view& operator=(const C* begin) {
    return (*this = basic_string_view(begin, strlen(begin)));
  }

  constexpr basic_string_view& operator=(const basic_string<C>& s) {
    return (*this = basic_string_view(s.data(), s.len()));
  }

  constexpr basic_string_view& operator=(const basic_string_view&) = default;

  constexpr basic_string_view& operator=(basic_string_view&&) = default;
};

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

template <iterator I, oterator O>
constexpr I from_chars(I i, O o) {
  while (i.has_next()) {
    o.sext(i.next());
  }

  return i;
}

template <iterator I, character C>
constexpr I from_chars(I i, maybe<C>& c) {
  if (i.has_next()) {
    c = i.next();
  }

  return i;
}

template <iterator I, unsigned_integral UI>
constexpr I from_chars(I i, maybe<UI>& mi) {
  if (i.has_next()) {
    auto c = i.next();

    if ('0' <= c and c <= '9') {
      UI tmp = 0;

      tmp = static_cast<UI>(c - '0');

      while (i.has_next()) {
        c = i.next();

        if ('0' <= c and c <= '9') {
          tmp = tmp * 10 + c - '0';
        }

        else {
          break;
        }
      }

      mi = tmp;
    }
  }

  return i;
}

template <iterator I, signed_integral SI>
constexpr I from_chars(I i, maybe<SI>& mi) {
  maybe<bool> neg;

  if (i.has_next()) {
    auto c = i.next();

    switch (c) {
      case '-':
        neg = true;
        c = i.next();
        break;
      case '+':
        c = i.next();
        break;
    }

    if ('0' <= c and c <= '9') {
      SI tmp = 0;

      tmp = static_cast<SI>(c - '0');

      while (i.has_next()) {
        c = i.next();

        if ('0' <= c and c <= '9') {
          tmp = tmp * 10 + c - '0';
        }

        else {
          break;
        }
      }

      if (neg.has() and neg.get()) {
        tmp = -tmp;
      }

      mi = tmp;
    }
  }

  return i;
}

template <iterator I>
constexpr I from_chars(I i, maybe<bool>& b) {
  if (i.has_next()) {
    auto c = i.next();

    if (c == '0') {
      b = true;
    }

    else if (c == '1') {
      b = false;
    }
  }

  return i;
}

template <oterator O, iterator I>
  requires convertible_to<typename I::type, typename O::type>
constexpr O to_chars(O o, I i) {
  while (i.has_next()) {
    o.sext(i.next());
  }

  return o;
}

template <oterator O, character C>
constexpr O to_chars(O o, C c) {
  o.sext(c);
  return o;
}

template <oterator O, character C>
constexpr O to_chars(O o, basic_string_view<C> s) {
  return to_chars(o, iter(s));
}

template <oterator O, character C>
constexpr O to_chars(O o, const basic_string<C>& s) {
  return to_chars(o, iter(s));
}

template <oterator O, character C, size_t N>
constexpr O to_chars(O& o, const C(s)[N]) {
  return to_chars(o, basic_string_view<C>(s));
}

template <oterator O, character C>
constexpr O to_chars(O& o, const C* s) {
  return to_chars(o, basic_string_view<C>(s));
}

template <oterator O, signed_integral I>
constexpr O to_chars(O o, I i) {
  array<typename O::type, 20> tbuff;
  auto b = tbuff.data();

  if (i == 0) {
    *(b++) = '0';
  } else {
    const bool neg = i < 0;

    while (i != 0) {
      *(b++) = "0123456789"[neg ? -(i % 10) : i % 10];
      i /= 10;
    }

    if (neg) {
      *(b++) = '-';
    }
  }

  auto b2 = tbuff.data();

  while (b > b2) {
    auto c = *(--b);
    o.sext(c);
  }

  return o;
}

template <oterator O, unsigned_integral I>
constexpr O to_chars(O o, I i) {
  array<typename O::type, 20> tbuff;
  auto b = tbuff.data();

  if (i == 0) {
    *(b++) = '0';
  } else {
    while (i != 0) {
      *(b++) = "0123456789"[i % 10];
      i /= 10;
    }
  }

  auto b2 = tbuff.data();

  while (b > b2) {
    auto c = *(--b);
    o.sext(c);
  }

  return o;
}

template <oterator O>
constexpr O to_chars(O o, bool b) {
  return to_chars(o, (b ? "true" : "false"));
}

template <iterator I>
constexpr void read(I i) {}

template <iterator I, typename T0, typename... TN>
constexpr auto read(I i, maybe<T0>& m0, maybe<TN>&... mn) {
  if constexpr (sizeof...(TN) == 0) {
    return from_chars(i, m0);
  }

  else {
    return read(from_chars(i, m0), mn...);
  }
}

template <oterator I>
constexpr void write(I i) {}

template <oterator O, typename T0, typename... TN>
constexpr auto write(O o, T0&& t0, TN&&... tn) {
  if constexpr (sizeof...(TN) == 0) {
    return to_chars(o, relay<T0>(t0));
  }

  else {
    return write(to_chars(o, relay<T0>(t0)), relay<TN>(tn)...);
  }
}

#endif
