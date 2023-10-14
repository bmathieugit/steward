#ifndef __steward_extract_hpp__
#define __steward_extract_hpp__

#include <stew/string.hpp>
#include <stew/tuple.hpp>

namespace stew {
template <typename T>
class extractor;

template <typename... T>
using extract_response = tuple<maybe<T>...>;

namespace impl {
template <size_t... I>
struct isequence {};

namespace impl {
template <size_t I0, size_t... In>
consteval auto make_isequence() {
  if constexpr (I0 == 0) {
    return isequence<I0, In...>{};
  } else {
    return make_isequence<I0 - 1, I0, In...>();
  }
}
}  // namespace impl

template <size_t N>
  requires(N > 0)
constexpr auto make_isequence() {
  return impl::make_isequence<N - 1>();
}

template <character C, typename T>
constexpr void extract_to_one_by_one(string_view<C> &input, string_view<C> &fmt,
                                     maybe<T> &m) {
  auto b = fmt.begin();
  auto e = fmt.end();

  auto i_b = input.begin();
  auto i_e = input.end();

  while (b != e && i_b != i_e && *b == *i_b) {
    ++b;
    ++i_b;
  }

  if (*b == '$') {
    ++b;
    input = string_view<C>(i_b, i_e);
    input = extractor<T>::to(input, m);
    fmt = string_view<C>(b, e);
  }
}

template <character C, typename... T>
constexpr void extract_to(string_view<C> input, string_view<C> fmt,
                          maybe<T> &...m) {
  (extract_to_one_by_one(input, fmt, m), ...);
}
}  // namespace impl

template <typename... T>
constexpr void extract_to(string_view<char> input, string_view<char> fmt,
                          maybe<T> &...m) {
  return impl::extract_to(input, fmt, m...);
}

// template <typename... T>
// constexpr void extract_to(
//     string_view<wchar_t> input,
//     format<wchar_t, T...> fmt,
//     extract_response<T...> &response)
// {
//   return impl::extract_to(
//       input, fmt.parts(), response,
//       impl::make_isequence<sizeof...(T)>());
// }

template <unsigned_integral I>
class extractor<I> {
 public:
  template <character C>
  constexpr static auto to(string_view<C> i, maybe<I> &mb) {
    auto b = begin(i);
    auto e = end(i);

    I res = 0;

    while (b != e && '0' <= *b && *b <= '9') {
      res = res * 10 + (*b - '0');
      ++b;
    }

    if (b != begin(i)) {
      mb = res;
    }

    return string_view<C>(b, end(i));
  }
};

template <signed_integral I>
class extractor<I> {
 public:
  template <character C>
  constexpr static auto to(string_view<C> i, maybe<I> &mb) {
    auto b = begin(i);
    auto e = end(i);

    I unit = 1;

    if (b != e) {
      if (*b == '-') {
        unit = -1;
        ++b;
      } else if (*b == '+') {
        ++b;
      }
    }

    I res = 0;

    while (b != e && '0' <= *b && *b <= '9') {
      res = res * 10 + (*b - '0');
      ++b;
    }

    if (b != begin(i)) {
      mb = unit * res;
    }

    return string_view<C>(b, end(i));
  }
};

template <>
class extractor<bool> {
 public:
  template <character C>
  constexpr static auto to(string_view<C> i, maybe<bool> &mb) {
    if (starts_with(i, str::view("true"))) {
      mb = true;
      return str::subv(i, 4);
    } else if (starts_with(i, str::view("false"))) {
      mb = false;
      return str::subv(i, 5);
    } else {
      return i;
    }
  }
};

}  // namespace stew

#endif
