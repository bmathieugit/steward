#ifndef __n_string_istream_hpp__
#define __n_string_istream_hpp__

#include <core/result.hpp>
#include <core/utils.hpp>
#include <core/concepts.hpp>

template <char_input_stream S, character C>
constexpr S& operator>>(S& s, maybe<C>& c) {
  if (s.has()) {
    c = s.next();
  }

  return s;
}

template <char_input_stream S, unsigned_integral I>
constexpr S& operator>>(S& s, maybe<I>& i) {
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

template <char_input_stream S, signed_integral I>
constexpr S& operator>>(S& s, maybe<I>& i) {
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

template <char_input_stream S>
constexpr S& operator>>(S& s, maybe<bool>& b) {
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

#endif