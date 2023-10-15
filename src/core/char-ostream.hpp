#ifndef __n_string_ostream_hpp__
#define __n_string_ostream_hpp__

#include <core/utils.hpp>
#include <core/concepts.hpp>
#include <core/array.hpp>
#include <core/collection.hpp>
#include <core/stream.hpp>
#include <core/string.hpp>

template <typename S>
concept char_output_stream = output_stream<S> and character<typename S::type>;

template <char_output_stream S, character C>
constexpr S& operator<<(S& o, C c) {
  o.add(c);
  return o;
}

template <char_output_stream S, iterator I>
constexpr S& operator<<(S& o, I i) {
  while (i.has()) {
    o << i.get();
    i.next();
  }

  return o;
}

template <char_output_stream S, collection C>
constexpr S& operator<<(S& o, const C& c) {
  return o << iter(c);
}

template <char_output_stream S, character C, size_t N>
constexpr S& operator<<(S& o, const C(s)[N]) {
  return o << iter(s);
}

template <char_output_stream S, character C>
constexpr S& operator<<(S& o, const C* s) {
  return o << iter(s);
}

template <char_output_stream S, signed_integral I>
constexpr S& operator<<(S& o, I i) {
  array<typename S::type, 20> tbuff;

  if (i == 0) {
    tbuff.add('0');
  } else {
    const bool neg = i < 0;
    I tmp = neg ? -i : i;

    while (tmp != 0) {
      tbuff.add("0123456789"[tmp % 10]);
      tmp /= 10;
    }

    if (neg) {
      tbuff.add('-');
    }
  }

  auto ibuff = riter(tbuff);

  while (ibuff.has()) {
    o.add(ibuff.get());
    ibuff.next();
  }

  return o;
}

template <char_output_stream S, unsigned_integral I>
constexpr S& operator<<(S& o, I i) {
  array<typename S::type, 20> tbuff;

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
    o.add(ibuff.get());
    ibuff.next();
  }

  return o;
}

template <char_output_stream S>
constexpr S& operator<<(S& o, bool b) {
  return o << (b ? "true" : "false");
}



#endif