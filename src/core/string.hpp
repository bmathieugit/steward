#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <core/array.hpp>
#include <core/core.hpp>
#include <core/cstring.hpp>
#include <core/vector.hpp>

template <typename S>
concept stringlike = collection<S> and character<typename S::type>;

template <character C>
using basic_string = vector<C>;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;

template <character C>
using basic_fixed_string = fixed_vector<C>;

using fixed_string = basic_fixed_string<char>;
using fixed_wstring = basic_fixed_string<wchar_t>;

template <character C, size_t N>
using basic_char_array = array<C, N>;

template <size_t N>
using char_array = basic_char_array<char, N>;

template <size_t N>
using wchar_array = basic_char_array<wchar_t, N>;

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

template <character C, size_t N>
using cstring = basic_char_array<C, N>;

#endif
