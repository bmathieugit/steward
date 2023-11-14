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
uof<N> to_hash(byte_t b, uof<N> h = fnvoffset<N>) {
  return (h ^ b) * fnvprime<N>;
}

template <size_t N, size_t M>
uof<N> to_hash(const byte_t (&bs)[M], uof<N> h = fnvoffset<N>) {
  for (size_t i = 0; i < M; ++i) {
    h = to_hash(bs[i], h);
  }

  return h;
}

template <size_t N>
uof<N> to_hash(const byte_t (&bs)[1], uof<N> h = fnvoffset<N>) {
  return to_hash(bs[0], h);
}

template <size_t N, character C>
uof<N> to_hash(const C c, uof<N> h = fnvoffset<N>) {
  return to_hash(static_cast<const byte_t[sizeof(C)]>(c), h);
}

template <size_t N, integral I>
uof<N> to_hash(const I i, uof<N> h = fnvoffset<N>) {
  return to_hash(static_cast<const byte_t[sizeof(I)]>(i), h);
}

template <size_t N, iterator I>
uof<N> to_hash(I i, uof<N> h = fnvoffset<N>) {
  while (i.has()) {
    to_hash(i.next(), h);
  }

  return h;
}

#endif