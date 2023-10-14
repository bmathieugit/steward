#ifndef __n_string_ostream_hpp__
#define __n_string_ostream_hpp__

#include <core/result.hpp>
#include <core/stream.hpp>
#include <core/utils.hpp>

template <typename S>
concept char_istream = istream<S> and character<typename S::type>;

template <char_istream S, character C>
constexpr S& operator>>(S& s, maybe<C>& c) {
  if (s.has()) {
    c = s.get();
    s.next();
  }

  return s;
}

template <char_istream S, unsigned_integral I>
constexpr S& operator>>(S& s, maybe<I>& i) {
  if (s.has()) {
    auto c = s.get();

    if ('0' <= c and c <= '9') {
      I tmp = 0;

      tmp = static_cast<I>(c - '0');
      s.next();

      while (s.has()) {
        c = s.get();

        if ('0' <= c and c <= '9') {
          tmp = tmp * 10 + c - '0';
          s.next();
        } else {
          break;
        }
      }

      i = tmp;
    }
  }

  return s;
}

template <char_istream S, signed_integral I>
constexpr S& operator>>(S& s, maybe<I>& i) {
  maybe<bool> neg;

  if (s.has()) {
    auto sign = s.get();

    switch (sign) {
      case '-':
        neg = true;
        s.next();
        break;
      case '+':
        neg = false;
        s.next();
        break;
    }
  }

  if (s.has()) {
    auto c = s.get();

    if ('0' <= c and c <= '9') {
      I tmp = 0;

      tmp = static_cast<I>(c - '0');
      s.next();

      while (s.has()) {
        c = s.get();

        if ('0' <= c and c <= '9') {
          tmp = tmp * 10 + c - '0';
          s.next();
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

template <char_istream S>
constexpr S& operator>>(S& s, maybe<bool>& b) {
  if (s.has()) {
    auto c = s.get();

    if (c == '0') {
      b = true;
    }

    else if (c == '1') {
      b = false;
    }
  }

  return s;
}

template <character C>
struct expected_char {
  C _expected;
};

template <char_istream S>
constexpr S& operator>>(S& s, maybe<expected_char<typename S::type>>& exc) {
  if (s.has() and exc.has() and exc.get()._expected == s.get()) {
    exc.get()._expected = s.get();
    s.next();

  }

  else {
    exc = maybe<expected_char<typename S::type>>();
  }

  return s;
}

#endif