#ifndef __n_string_hpp__
#define __n_string_hpp__

#include <core/array.hpp>
#include <core/cstring.hpp>
#include <core/utils.hpp>
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

#endif
