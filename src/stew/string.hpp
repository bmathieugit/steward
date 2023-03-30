#ifndef __steward_string_hpp__
#define __steward_string_hpp__

#include <stew/meta.hpp>
#include <stew/meta/iterable.hpp>
#include <stew/vector.hpp>
#include <stew/view.hpp>

namespace stew {
template <character C>
using string_view = pointer_view<const C>;

template <character C, size_t N>
using static_string = static_vector<C, N>;

template <character C>
using string = vector<C>;

template <character C>
using ext_string = ext_vector<C>;

template <typename T, typename C>
concept string_view_like = like<T, string_view<C>>;

namespace str {

template <character C>
constexpr size_t len(const C *s) {
  const C *cur = s;

  if (cur != nullptr)
    while (*cur != '\0') ++cur;

  return cur - s;
}

template <character C>
constexpr string_view<C> view(const C *s) {
  return string_view<C>(s, s + len(s));
}

template <character C, size_t N>
constexpr static_string<C, N - 1> stic(const C (&s)[N]) {
  return static_string<C, N - 1>(string_view<C>(s, s + N));
}

template <character C>
constexpr string<C> str(const C *s) {
  return string<C>(view<C>(s));
}

template <character C>
constexpr ext_string<C> ext(const C *s) {
  return ext_string<C>(view(s));
}

template <iterable S0, iterable S1>
constexpr auto cat(const S0 &s0, const S1 &s1) {
  string<rm_cvref<decltype(s0.iter().next())>> s(s0.size() + s1.size());
  copy(s0, s);
  copy(s1, s);
  return s;
}

template <iterable S0, iterable S1>
constexpr int cmp(const S0 &s0, const S1 &s1)
  requires requires {
             s0.size();
             s1.size();
           }
{
  if (s0.size() > s1.size()) {
    return 1;
  } else if (s0.size() < s1.size()) {
    return -1;
  } else {
    auto i0 = s0.iter();
    auto i1 = s1.iter();

    rm_cvref<decltype(i0.next())> c0 = 0;
    rm_cvref<decltype(i1.next())> c1 = 0;

    while (i0.has_next() && i1.has_next()) {
      c0 = i0.next();
      c1 = i1.next();

      if (c0 != c1) {
        break;
      }
    }

    return c0 - c1;
  }
}

}  // namespace str
}  // namespace stew

#endif
