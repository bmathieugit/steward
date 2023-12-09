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
constexpr uof<N> to_hash(byte_t b, uof<N> h = fnvoffset<N>) {
  return (h ^ b) * fnvprime<N>;
}

template <size_t N, size_t M>
constexpr uof<N> to_hash(const byte_t (&bs)[M], uof<N> h = fnvoffset<N>) {
  for (size_t i = 0; i < M; ++i) {
    h = to_hash<N>(bs[i], h);
  }

  return h;
}

template <size_t N>
constexpr uof<N> to_hash(wchar_t c, uof<N> h = fnvoffset<N>) {
  return to_hash<N>(static_cast<const byte_t[sizeof(wchar_t)]>(c), h);
}

template <size_t N, integral I>
constexpr uof<N> to_hash(const I i, uof<N> h = fnvoffset<N>) {
  return to_hash<N>(static_cast<const byte_t[sizeof(I)]>(i), h);
}

template <size_t N, istream I>
constexpr uof<N> to_hash(I i, uof<N> h = fnvoffset<N>) {
  while (i.has()) {
    h = to_hash<N>(i.next(), h);
  }

  return h;
}

#endif