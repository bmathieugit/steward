#ifndef __n_cstring_hpp__
#define __n_cstring_hpp__

#include <core/core.hpp>

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
  using position = size_t;
  using type = C;

 private:
  const char* _start;
  const char* _data;

 public:
  constexpr null_terminating_string_forward_input_stream(const C* data)
      : _start(data), _data(data) {}

 public:
  constexpr bool has() const { return *_data != '\0'; }
  constexpr auto next() -> decltype(auto) { return (*(_data++)); }
  // constexpr position pos() const { return _data - _start; }
};

template <character C>
constexpr auto istream(const C* s) {
  return null_terminating_string_forward_input_stream(s);
}

#endif