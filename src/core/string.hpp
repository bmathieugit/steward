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
 public:
  using type = C;
  using reference = C&;
  using const_reference = const C&;
  using move_reference = C&&;
  using pointer = C*;
  using const_pointer = const C*;

 private:
  allocator<C> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  pointer _data = nullptr;

 public:
  constexpr ~basic_string() {
    _alloc.destroy(_data, _len);
    _alloc.deallocate(_data, _max);
  }

  constexpr basic_string() = default;

  constexpr basic_string(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(max + 1)) {
    _data[_len] = 0;
  }

  template <iterator I>
    requires distanciable<I>
  constexpr basic_string(I b, I e) : basic_string(e - b) {
    while (b != e) {
      this->add(*(b++));
    }
  }

  template <iterator I>
    requires(not distanciable<I>)
  constexpr basic_string(I b, I e) {
    while (b != e) {
      this->add(*(b++));
    }
  }

  constexpr basic_string(basic_string_view<C> s)
      : basic_string(begin(s), end(s)) {}

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

  constexpr pointer data() { return _data; }

  constexpr const_pointer data() const { return _data; }

  constexpr void clear() {
    _alloc.destroy(_data, _len);
    _len = 0;

    if (_data != nullptr) {
      _data[0] = 0;
    }
  }

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

      _alloc.deallocate(_data, _max);
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
constexpr auto begin(const basic_string<C>& s) {
  return s.data();
}

template <character C>
constexpr auto end(const basic_string<C>& s) {
  return s.data() + s.len();
}

template <character C>
constexpr auto begin(basic_string<C>& s) {
  return s.data();
}

template <character C>
constexpr auto end(basic_string<C>& s) {
  return s.data() + s.len();
}

template <character C>
class basic_string_view : public span<const C> {
 public:
  using type = C;
  using reference = C&;
  using const_reference = const C&;
  using move_reference = C&&;
  using pointer = C*;
  using const_pointer = const C*;

 public:
  constexpr basic_string_view() = default;
  constexpr basic_string_view(const_pointer begin, const_pointer end)
      : span<const C>(begin, end) {}

  constexpr basic_string_view(const_pointer begin, size_t len)
      : span<const C>(begin, len) {}

  constexpr basic_string_view(const_pointer begin)
      : span<const C>(begin, strlen(begin)) {}

  constexpr basic_string_view(const basic_string<C>& s)
      : span<const C>(begin(s), s.len()) {}

  constexpr basic_string_view(const basic_string_view&) = default;

  constexpr basic_string_view(basic_string_view&&) = default;

  constexpr basic_string_view& operator=(const_pointer begin) {
    return (*this = basic_string_view(begin, strlen(begin)));
  }

  constexpr basic_string_view& operator=(const basic_string<C>& s) {
    return (*this = basic_string_view(begin(s), s.len()));
  }

  constexpr basic_string_view& operator=(const basic_string_view&) = default;

  constexpr basic_string_view& operator=(basic_string_view&&) = default;
};

template <character C>
constexpr auto begin(basic_string_view<C> s) {
  return s.data();
}

template <character C>
constexpr auto end(const basic_string_view<C> s) {
  return s.data() + s.len();
}

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;

template <character C>
class basic_string_istream {
 public:
  using type = C;

 private:
  const C* _b = nullptr;
  const C* _e = nullptr;

 public:
  constexpr basic_string_istream(basic_string_view<C> s)
      : _b(begin(s)), _e(end(s)) {}

  constexpr basic_string_istream(const basic_string<C>& s)
      : basic_string_istream(basic_string_view<C>(s)) {}

  constexpr basic_string_istream(const C* s)
      : basic_string_istream(basic_string_view<C>(s)) {}

 public:
  constexpr C next() { return *(_b++); }

  constexpr bool has() { return _b != _e; }
};

using string_istream = basic_string_istream<char>;
using wstring_istream = basic_string_istream<wchar_t>;

template <character C>
class basic_string_ostream {
 public:
  using type = C;

 private:
  basic_string<C> _str;

 public:
  constexpr void add(C c) { _str.add(c); }

  constexpr basic_string<C> str() {
    auto tmp = move(_str);
    return tmp;
  }
};

using string_ostream = basic_string_ostream<char>;
using wstring_ostream = basic_string_ostream<wchar_t>;

template <character C>
class basic_string_stream {
 public:
  using type = C;

 private:
  basic_string<C> _data;
  size_t _idx = 0;

 public:
  constexpr basic_string_stream() = default;
  constexpr basic_string_stream(basic_string_view<C> s)
      : _data(begin(s), end(s)) {}

  constexpr basic_string_stream(const basic_string<C>& s)
      : basic_string_stream(basic_string_view<C>(s)) {}

  constexpr basic_string_stream(const C* s)
      : basic_string_stream(basic_string_view<C>(s)) {}

 public:
  constexpr C next() { return _data.at(_idx++); }

  constexpr bool has() { return _data.has(_idx); }

  constexpr void add(C c) { _data.add(c); }

  constexpr basic_string<C> str() {
    auto tmp = move(_data);
    return tmp;
  }
};

using string_stream = basic_string_stream<char>;
using wstring_stream = basic_string_stream<wchar_t>;

template <istream I, ostream O>
constexpr I& from_chars(I& s, O& o) {
  while (s.has()) {
    o.add(s.next());
  }

  return s;
}

template <istream I, character C>
constexpr I& from_chars(I& s, maybe<C>& c) {
  if (s.has()) {
    c = s.next();
  }

  return s;
}

template <istream I, unsigned_integral UI>
constexpr I& from_chars(I& s, maybe<UI>& i) {
  if (s.has()) {
    auto c = s.next();

    if ('0' <= c and c <= '9') {
      UI tmp = 0;

      tmp = static_cast<UI>(c - '0');

      while (s.has()) {
        c = s.next();

        if ('0' <= c and c <= '9') {
          tmp = tmp * 10 + c - '0';
        } else {
          break;
        }
      }

      i = tmp;
    }
  }

  return s;
}

template <istream I, signed_integral SI>
constexpr I& from_chars(I& s, maybe<SI>& i) {
  maybe<bool> neg;

  if (s.has()) {
    auto c = s.next();

    switch (c) {
      case '-':
        neg = true;
        c = s.next();
        break;
      case '+':
        c = s.next();
        break;
    }

    if ('0' <= c and c <= '9') {
      SI tmp = 0;

      tmp = static_cast<SI>(c - '0');

      while (s.has()) {
        c = s.next();

        if ('0' <= c and c <= '9') {
          tmp = tmp * 10 + c - '0';
        } else {
          break;
        }
      }

      if (neg.has() and neg.get()) {
        tmp = -tmp;
      }

      i = tmp;
    }
  }

  return s;
}

template <istream I>
constexpr I& from_chars(I& s, maybe<bool>& b) {
  if (s.has()) {
    auto c = s.next();

    if (c == '0') {
      b = true;
    }

    else if (c == '1') {
      b = false;
    }
  }

  return s;
}

template <ostream O, istream I>
  requires convertible_to<typename I::type, typename O::type>
constexpr O& to_chars(O& o, I& i) {
  while (i.has()) {
    o.add(i.next());
  }

  return o;
}

template <ostream O, character C>
constexpr O& to_chars(O& o, C c) {
  o.add(c);
  return o;
}

template <ostream O, character C>
constexpr O& to_chars(O& o, basic_string_view<C> s) {
  for_each(begin(s), end(s), [&o](C c) { o.add(c); });
  return o;
}

template <ostream O, character C>
constexpr O& to_chars(O& o, const basic_string<C>& s) {
  return to_chars(o, basic_string_view<C>(s));
}

template <ostream O, character C, size_t N>
constexpr O& to_chars(O& o, const C(s)[N]) {
  return to_chars(o, basic_string_view<C>(s));
}

template <ostream O, character C>
constexpr O& to_chars(O& o, const C* s) {
  return to_chars(o, basic_string_view<C>(s));
}

template <ostream O, signed_integral I>
constexpr O& to_chars(O& o, I i) {
  array<typename O::type, 20> tbuff;
  auto b = begin(tbuff);

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

  auto b2 = begin(tbuff);

  while (b > b2) {
    auto c = *(--b);
    o.add(c);
  }

  return o;
}

template <ostream O, unsigned_integral I>
constexpr O& to_chars(O& o, I i) {
  array<typename O::type, 20> tbuff;
  auto b = begin(tbuff);

  if (i == 0) {
    *(b++) = '0';
  } else {
    while (i != 0) {
      *(b++) = "0123456789"[i % 10];
      i /= 10;
    }
  }

  auto b2 = begin(tbuff);

  while (b > b2) {
    auto c = *(--b);
    o.add(c);
  }

  return o;
}

template <ostream O>
constexpr O& to_chars(O& o, bool b) {
  return to_chars(o, (b ? "true" : "false"));
}

template <istream I, typename... C>
constexpr void read(I i, maybe<C>&... mt) {
  (from_chars(i, mt), ...);
}

template <ostream O, typename... C>
constexpr void write(O o, C&&... t) {
  (to_chars(o, relay<C>(t)), ...);
}

#endif
