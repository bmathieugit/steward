#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <core/algorithm.hpp>
#include <core/allocator.hpp>
#include <core/core.hpp>
#include <core/result.hpp>
#include <core/vector.hpp>

template <typename C>
concept char_collection = collection<C> and character<typename C::type>;

template <typename C>
concept char_iterator = character<typename C::type> and iterator<C>;

template <typename C>
concept char_oterator = character<typename C::type> and oterator<C>;

template <character C, size_t N>
class basic_static_string {
 public:
  using type = C;
  using position = size_t;

 private:
  C _data[N + 1];
  size_t _len = 0;

 public:
  constexpr ~basic_static_string() = default;
  constexpr basic_static_string() = default;
  constexpr basic_static_string(const C (&data)[N]) {
    for (size_t i = 0; i < N; ++i) {
      add(data[i]);
    }
  }

 public:
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return N; }
  constexpr auto full() const { return _len == N; }

  constexpr bool has(position p) const { return p < _len; }
  constexpr C& at(position p) { return _data[p]; }
  constexpr const C& at(position p) const { return _data[p]; }
  constexpr auto data() const { return _data; }

 public:
  constexpr void clear() {
    _data[0] = 0;
    _len = 0;
  }

  constexpr bool add(type c, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (p <= _len and not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = _data[i - 1];
      }

      _data[p] = c;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool exchange(position p1, position p2) {
    if (has(p1) and has(p2)) {
      auto tmp1 = _data[p1];
      _data[p1] = _data[p2];
      _data[p2] = tmp1;

      return true;
    }

    else {
      return false;
    }
  }

  constexpr bool modify(type c, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = c;

      if (c == 0) {
        _len = p;
      }

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

      _data[_len - 1] = 0;
      _len -= 1;

      return true;
    }
  }
};

template <size_t N>
using char_static_vector = basic_static_string<char, N>;

template <size_t N>
using wchar_static_vector = basic_static_string<wchar_t, N>;

template <character C, size_t N>
constexpr auto iter(const basic_static_string<C, N>& a) {
  return index_based_iterator(a);
}

template <character C, size_t N>
constexpr auto riter(const basic_static_string<C, N>& a) {
  return index_based_riterator(a);
}

template <character C, size_t N>
constexpr auto oter(basic_static_string<C, N>& a) {
  return index_based_oterator(a);
};

template <typename C>
class basic_fixed_string {
 public:
  using type = C;
  using position = size_t;

 private:
  allocator<C> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  C* _data = nullptr;

 public:
  constexpr ~basic_fixed_string() {
    _alloc.deallocate(_data);
    _data = nullptr;
    _max = 0;
    _len = 0;
  }

  constexpr basic_fixed_string() = default;

  constexpr basic_fixed_string(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(_max + 1)) {}

  constexpr basic_fixed_string(const basic_fixed_string& v)
      : basic_fixed_string(v._max) {
    copy(index_based_iterator(v), index_based_oterator(*this));
  }

  constexpr basic_fixed_string(basic_fixed_string&& v)
      : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  constexpr basic_fixed_string& operator=(const basic_fixed_string& v) {
    if (this != &v) {
      _alloc.deallocate(_data);
      _data = nullptr;
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._max + 1);
        _max = v._max;
      }

      copy(index_based_iterator(v), index_based_oterator(*this));
    }

    return *this;
  }

  constexpr basic_fixed_string& operator=(basic_fixed_string&& v) {
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
  constexpr auto data() const { return _data; }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr C& at(position p) { return _data[p]; }
  constexpr const C& at(position p) const { return _data[p]; }

  constexpr void clear() {
    if (_len != 0 and _data != nullptr) {
      _data[0] = 0;
    }

    _len = 0;
  }

  constexpr bool add(type c, position p = max_of<decltype(_len)>) {
    p = p >= max_of<decltype(_len)> ? _len : p;

    if (p <= _len and not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = _data[i - 1];
      }

      _data[p] = c;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool exchange(position p1, position p2) {
    if (has(p1) and has(p2)) {
      auto tmp1 = _data[p1];
      _data[p1] = _data[p2];
      _data[p2] = tmp1;
      return true;
    }

    else {
      return false;
    }
  }

  constexpr bool modify(type c, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = c;

      if (c == 0) {
        _len = p;
      }

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

      _data[_len - 1] = 0;
      _len -= 1;

      return true;
    }
  }
};

template <character C>
constexpr auto iter(const basic_fixed_string<C>& v) {
  return index_based_iterator(v);
}
template <character C>
constexpr auto riter(const basic_fixed_string<C>& v) {
  return index_based_riterator(v);
}
template <character C>
constexpr auto oter(basic_fixed_string<C>& v) {
  return index_based_oterator(v);
}

using fixed_string = basic_fixed_string<char>;
using fixed_wstring = basic_fixed_string<wchar_t>;

template <character C>
class basic_string {
 public:
  using type = C;
  using position = size_t;

 private:
  allocator<type> _alloc;
  size_t _max = 0;
  size_t _len = 0;
  C* _data = nullptr;

 public:
  constexpr ~basic_string() = default;
  constexpr basic_string() = default;
  constexpr basic_string(size_t max)
      : _max(max), _len(0), _data(_alloc.allocate(max + 1)) {}

  constexpr basic_string(const basic_string& v) : basic_string(v._max) {
    copy(index_based_iterator(v), index_based_oterator(*this));
  }

  constexpr basic_string(basic_string&& v)
      : _max(v._max), _len(v._len), _data(v._data) {
    v._max = 0;
    v._len = 0;
    v._data = nullptr;
  }

  template <iterator I>
  constexpr basic_string(I i) : basic_string() {
    copy(i, index_based_oterator(*this));
  }

  constexpr basic_string& operator=(const basic_string& v) {
    if (this != &v) {
      _alloc.deallocate(_data);
      _data = nullptr;
      _len = 0;
      _max = 0;

      if (v._len != 0) {
        _data = _alloc.allocate(v._max + 1);
        _max = v._max;
      }

      copy(index_based_iterator(v), index_based_oterator(*this));
    }

    return *this;
  }

  constexpr basic_string& operator=(basic_string&& v) {
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
  constexpr basic_string& operator=(I i) {
    clear();
    copy(i, index_based_oterator(*this));
    return *this;
  }

 public:
  constexpr auto data() const { return _data; }
  constexpr auto len() const { return _len; }
  constexpr auto empty() const { return _len == 0; }
  constexpr auto max() const { return _max; }
  constexpr auto full() const { return _len == _max; }

 public:
  constexpr bool has(position p) const { return p < _len; }
  constexpr C& at(position p) { return _data[p]; }
  constexpr const C& at(position p) const { return _data[p]; }

  constexpr void clear() {
    if (_len != 0 and _data != nullptr) {
      _data[0] = 0;
    }

    _len = 0;
  }

  constexpr bool add(type c, position p = max_of<size_t>) {
    p = p >= max_of<size_t> ? _len : p;

    if (full()) {
      _max = _max * 2 + 10;
      auto dtmp = _alloc.allocate(_max + 1);

      for (size_t i = 0; i < _len; ++i) {
        dtmp[i] = _data[i];
      }

      _data = dtmp;
    }

    if (not full()) {
      for (size_t i = _len; i > p; --i) {
        _data[i] = _data[i - 1];
      }

      _data[p] = c;
      ++_len;
      return true;
    }

    return false;
  }

  constexpr bool exchange(position p1, position p2) {
    if (has(p1) and has(p2)) {
      auto tmp1 = _data[p1];
      _data[p1] = _data[p2];
      _data[p2] = tmp1;
      return true;
    }

    else {
      return false;
    }
  }

  constexpr bool modify(type c, position p) {
    if (p >= _len) {
      return false;
    } else {
      _data[p] = c;

      if (c == 0) {
        _len = p;
      }

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

      _data[_len - 1] = 0;
      _len -= 1;

      return true;
    }
  }
};

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <typename C>
constexpr auto iter(const basic_string<C>& v) {
  return index_based_iterator(v);
}

template <typename C>
constexpr auto riter(const basic_string<C>& v) {
  return index_based_riterator(v);
}

template <typename C>
constexpr auto oter(basic_string<C>& v) {
  return index_based_oterator(v);
}

template <character C>
class basic_string_iterator {
 public:
  using type = C;

  /* FIXME: to private*/
 public:
  const C* _data = nullptr;

 public:
  constexpr basic_string_iterator() = default;
  constexpr basic_string_iterator(const basic_string_iterator&) = default;
  constexpr basic_string_iterator(basic_string_iterator&&) = default;
  constexpr basic_string_iterator(const C* data) : _data(data) {}

  template <size_t N>
  constexpr basic_string_iterator(const basic_static_string<C, N>& s)
      : _data(s.data()) {}
  constexpr basic_string_iterator(const basic_fixed_string<C>& s)
      : _data(s.data()) {}
  constexpr basic_string_iterator(const basic_string<C>& s) : _data(s.data()) {}
  constexpr basic_string_iterator& operator=(const basic_string_iterator&) =
      default;
  constexpr basic_string_iterator& operator=(basic_string_iterator&&) = default;

 public:
  constexpr bool has() const { return _data != nullptr and *_data != '\0'; }
  constexpr auto next() -> decltype(auto) { return (*(_data++)); }
};

template <character C>
constexpr auto iter(const C* s) {
  return basic_string_iterator<C>(s);
}

using string_iterator = basic_string_iterator<char>;
using wstring_iterator = basic_string_iterator<wchar_t>;

template <char_iterator S, character C>
constexpr S& from_chars(S& s, maybe<C>& c) {
  if (s.has()) {
    c = s.next();
  }

  return s;
}

template <char_iterator S, unsigned_integral I>
constexpr S& from_chars(S& s, maybe<I>& i) {
  if (s.has()) {
    auto c = s.next();

    if ('0' <= c and c <= '9') {
      I tmp = 0;

      tmp = static_cast<I>(c - '0');

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

template <char_iterator S, signed_integral I>
constexpr S& from_chars(S& s, maybe<I>& i) {
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
      I tmp = 0;

      tmp = static_cast<I>(c - '0');

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

template <char_iterator S>
constexpr S& from_chars(S& s, maybe<bool>& b) {
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

template <oterator O, character C>
constexpr O& to_chars(O& o, C c) {
  o.add(c);
  return o;
}

template <char_oterator O, char_iterator I>
constexpr O& to_chars(O& o, I i) {
  while (i.has()) {
    o.add(i.next());
  }

  return o;
}

template <char_oterator S, collection C>
constexpr S& to_chars(S& o, const C& c) {
  return to_chars(o, iter(c));
}

template <char_oterator S, character C, size_t N>
constexpr S& to_chars(S& o, const C(s)[N]) {
  return to_chars(o, iter(s));
}

template <char_oterator S, character C>
constexpr S& to_chars(S& o, const C* s) {
  return to_chars(o, iter(s));
}

template <char_oterator S, signed_integral I>
constexpr S& to_chars(S& o, I i) {
  basic_static_string<typename S::type, 20> tbuff;

  if (i == 0) {
    tbuff.add('0');
  } else {
    const bool neg = i < 0;

    while (i != 0) {
      tbuff.add("0123456789"[neg ? -(i % 10) : i % 10]);
      i /= 10;
    }

    if (neg) {
      tbuff.add('-');
    }
  }

  auto ibuff = riter(tbuff);

  while (ibuff.has()) {
    o.add(ibuff.next());
  }

  return o;
}

template <char_oterator S, unsigned_integral I>
constexpr S& to_chars(S& o, I i) {
  basic_static_string<typename S::type, 20> tbuff;

  if (i == 0) {
    tbuff.add('0');
  } else {
    while (i != 0) {
      tbuff.add("0123456789"[i % 10]);
      i /= 10;
    }
  }

  auto ibuff = riter(tbuff);

  while (ibuff.has()) {
    o.add(ibuff.next());
  }

  return o;
}

template <char_oterator S>
constexpr S& to_chars(S& o, bool b) {
  return to_chars(o, (b ? "true" : "false"));
}

template <char_iterator I, typename... T>
constexpr void read(I i, maybe<T>&... mt) {
  (from_chars(i, mt), ...);
}

template <char_oterator O, typename... T>
constexpr void write(O o, const T&... t) {
  (to_chars(o, t), ...);
}

#endif
