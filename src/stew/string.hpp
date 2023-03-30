#ifndef __steward_string_hpp__
#define __steward_string_hpp__

#include <stew/meta.hpp>
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

template <character C>
constexpr string<C> str(const C *s) {
  return string<C>(view<C>(s));
}

template <character C>
constexpr ext_string<C> ext(const C *s) {
  return ext_string<C>(view(s));
}


/*
template <character C>
constexpr fixed_string<C> cat(const C *s0, const C *s1) {
  string_view<C> v0 = view(s0);
  string_view<C> v1 = view(s1);
  fixed_string<C> s(v0.size() + v1.size());
  copy(v0, pushing(s));
  copy(v1, pushing(s));
  return s;
}

template <character C>
constexpr fixed_string<C> cat(string_view<C> s0, string_view<C> s1) {
  fixed_string<C> s(s0.size() + s1.size());
  copy(s0, pushing<C>(s));
  copy(s1, pushing<C>(s));
  return s;
}

template <character C>
constexpr auto cat(const string_view_like<C> auto &s0, const C *s1)
    -> decltype(auto) {
  string_view<C> v1(s1, s1 + len(s1));
  rm_cvref<decltype(s0)> s(s0.size() + v1.size());
  copy(s0, pushing(s));
  copy(v1, pushing(s));
  return s;
}

template <character C>
constexpr auto cat(const string_view_like<C> auto &s0,
                   const string_view_like<C> auto &s1) -> decltype(auto) {
  rm_cvref<decltype(s0)> s(s0.size() + s1.size());
  copy(s0, pushing(s));
  copy(s1, pushing(s));
  return s;
}

template <character C>
constexpr int cmp(const C *s0, const C *s1) {
  if (s0 != nullptr && s1 != nullptr)
    while (*s0 != '\0' && *s1 != '\0' && *s0 == *s1) {
      ++s0;
      ++s1;
    }

  C c0 = s0 == nullptr ? 0 : *s0;
  C c1 = s1 == nullptr ? 0 : *s1;

  return c0 - c1;
}

template <character C>
constexpr int cmp(const string_view_like<C> auto &s0,
                  const string_view_like<C> auto &s1) {
  return cmp(s0.begin(), s1.begin());
}

template <character C>
constexpr string_view<C> subv(string_view<C> s, size_t from) {
  return string_view<C>(begin(s) + from, end(s));
}

template <character C>
constexpr string_view<C> subv(string_view<C> s, size_t from, size_t n) {
  return string_view<C>(begin(s) + from, begin(s) + from + n);
}
*/
}  // namespace str

template <character C>
struct after_pair {
  bool _found;
  string_view<C> _aft;
};

template <character C>
constexpr after_pair<C> after(string_view<C> input, string_view<C> sep) {
  auto pos = find(input, sep);

  return after_pair<C>{pos != end(input), {pos + sep.size(), end(input)}};
}

template <character C>
struct around_pair {
  bool _found;
  string_view<C> _bef;
  string_view<C> _aft;
};

template <character C>
constexpr around_pair<C> around(string_view<C> input, string_view<C> sep) {
  auto pos = find(input, sep);

  if (pos != end(input)) {
    return around_pair<C>{
        true, {begin(input), pos}, {pos + sep.size(), end(input)}};
  } else {
    return around_pair<C>{
        false, {begin(input), begin(input)}, {begin(input), end(input)}};
  }
}

template <character C>
constexpr around_pair<C> around(string_view<C> input, C sep) {
  auto pos = find(input, sep);

  if (pos != end(input)) {
    return around_pair<C>{true, {begin(input), pos}, {pos + 1, end(input)}};
  } else {
    return around_pair<C>{
        false, {begin(input), begin(input)}, {begin(input), end(input)}};
  }
}

}  // namespace stew

#endif
