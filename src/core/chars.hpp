#ifndef __stew_core_chars_hpp__
#define __stew_core_chars_hpp__

#include <core/core.hpp>
#include <core/string.hpp>

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