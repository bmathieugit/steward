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


#endif
