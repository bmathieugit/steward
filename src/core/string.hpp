#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <core/core.hpp>
#include <core/vector.hpp>

template <character C, size_t N>
using basic_char_static_vector = static_vector<C, N>;

template <size_t N>
using char_static_vector = basic_char_static_vector<char, N>;

template <size_t N>
using wchar_static_vector = basic_char_static_vector<wchar_t, N>;

template <character C>
using basic_fixed_string = fixed_vector<C>;

using fixed_string = basic_fixed_string<char>;
using fixed_wstring = basic_fixed_string<wchar_t>;

template <character C>
using basic_string = vector<C>;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <character C>
constexpr size_t strlen(const C* s) {
  size_t i = 0;
  if (s != nullptr)
    while (s[i] != '\0')
      ++i;
  return i;
}

template <character C>
class null_terminating_string_forward_input_stream {
 public:
  using type = C;

 private:
  const char* _data;

 public:
  constexpr null_terminating_string_forward_input_stream(const C* data)
      : _data(data) {}

 public:
  constexpr bool has() const { return *_data != '\0'; }
  constexpr auto next() -> decltype(auto) { return (*(_data++)); }
};

template <character C>
constexpr auto iter(const C* s) {
  return null_terminating_string_forward_input_stream(s);
}

template <character C, size_t N>
using cstring = basic_char_static_vector<C, N>;

template <char_iterator S, character C>
constexpr S& extract(S& s, maybe<C>& c) {
  if (s.has()) {
    c = s.next();
  }

  return s;
}

template <char_iterator S, unsigned_integral I>
constexpr S& extract(S& s, maybe<I>& i) {
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
constexpr S& extract(S& s, maybe<I>& i) {
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
constexpr S& extract(S& s, maybe<bool>& b) {
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
constexpr O& insert(O& o, C c) {
  o.add(c);
  return o;
}

template <char_oterator O, char_iterator I>
constexpr O& insert(O& o, I i) {
  while (i.has()) {
    o.add(i.next());
  }

  return o;
}

template <char_oterator S, collection C>
constexpr S& insert(S& o, const C& c) {
  return insert(o, iter(c));
}

template <char_oterator S, character C, size_t N>
constexpr S& insert(S& o, const C(s)[N]) {
  return insert(o, iter(s));
}

template <char_oterator S, character C>
constexpr S& insert(S& o, const C* s) {
  return insert(o, iter(s));
}
#include <stdio.h>
template <char_oterator S, signed_integral I>
constexpr S& insert(S& o, I i) {
  static_vector<typename S::type, 20> tbuff;

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
constexpr S& insert(S& o, I i) {
  static_vector<typename S::type, 20> tbuff;

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
constexpr S& insert(S& o, bool b) {
  return insert(o, (b ? "true" : "false"));
}

#endif
