#ifndef __stew_core_read_write_hpp__
#define __stew_core_read_write_hpp__

#include <core/chars.hpp>
#include <core/core.hpp>
#include <core/result.hpp>

template <char_iterator I, typename... T>
constexpr void read(I i, maybe<T>&... mt) {
  (from_chars(i, mt), ...);
}

template <char_oterator O, typename... T>
constexpr void write(O o, const T&... t) {
  (to_chars(o, t), ...);
}

#endif