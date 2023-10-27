#ifndef __stew_core_fnv_ostream_hpp__
#define __stew_core_fnv_ostream_hpp__

#include <core/core.hpp>

template <size_t N>
constexpr size_t fnvprime = 0;

template <>
constexpr size_t fnvprime<32> = 16777619;

template <>
constexpr size_t fnvprime<64> = 1099511628211UL;

template <size_t N>
constexpr size_t fnvoffset = 0;

template <>
constexpr size_t fnvoffset<32> = 2166136261;

template <>
constexpr size_t fnvoffset<64> = 14695981039346656037UL;

template <size_t N>
class fnv_ostream {
 public:
  using type = byte_t;

 private:
  uof<N> _hash = fnvoffset<N>;

 public:
  constexpr fnv_ostream() = default;

 public:
  constexpr bool add(byte_t b) {
    _hash = (_hash ^ b) * fnvprime<N>;
    return true;
  }

  constexpr auto hash() const { return _hash; }
};

template <size_t N, size_t M>
fnv_ostream<N>& operator<<(fnv_ostream<N>& o, const byte_t (&bs)[M]) {
  for (size_t i = 0; i < M; ++i) {
    o.add(bs[i]);
  }

  return o;
}

template <size_t N>
fnv_ostream<N>& operator<<(fnv_ostream<N>& o, const byte_t (&bs)[1]) {
  o.add(bs[0]);
  return o;
}

template <size_t N, character C>
fnv_ostream<N>& operator<<(fnv_ostream<N>& o, const C c) {
  return o << static_cast<const byte_t[sizeof(C)]>(c);
}

template <size_t N, integral I>
fnv_ostream<N>& operator<<(fnv_ostream<N>& o, const I i) {
  return o << static_cast<const byte_t[sizeof(I)]>(i);
}

template <size_t N, collection C>
fnv_ostream<N>& operator<<(fnv_ostream<N>& o, const C& c) {
  auto ord = c.ord();

  while (ord.has()) {
    o << c.at(ord.next());
  }

  return o;
}

template <size_t N, input_stream I>
fnv_ostream<N>& operator<<(fnv_ostream<N>& o, I i) {
  while (i.has()) {
    o << i.next();
  }

  return o;
}

#endif