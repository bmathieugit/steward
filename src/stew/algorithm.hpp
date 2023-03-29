#ifndef __steward_algorithm_hpp__
#define __steward_algorithm_hpp__

#include <stew/meta.hpp>

namespace stew {
template <typename T>
concept forward_incrementable_iterator = (
    requires(T t) {
      { t == t } -> convertible_to<bool>;
    } &&
    requires(T t) {
      { t != t } -> convertible_to<bool>;
    } &&
    requires(T t) {
      { ++t } -> same_one_of<T &, T>;
    } &&
    requires(T t) {
      { t++ } -> same_one_of<T &, T>;
    });

template <typename T>
concept backward_incrementable_iterator = (
    requires(T t) {
      { t == t } -> convertible_to<bool>;
    } &&
    requires(T t) {
      { t != t } -> convertible_to<bool>;
    } &&
    requires(T t) {
      { --t } -> same_one_of<T &, T>;
    } &&
    requires(T t) {
      { t-- } -> same_one_of<T &, T>;
    });

template <typename T>
concept input_iterator = (forward_incrementable_iterator<T> &&
                          (
                              requires(T t) {
                                { *t } -> const_double_reference_eq;
                              } ||
                              requires(T t) {
                                { *t } -> double_reference_eq;
                              } ||
                              requires(T t) {
                                { *t } -> reference_eq;
                              } ||
                              requires(T t) {
                                { *t } -> const_reference_eq;
                              } ||
                              requires(T t) {
                                { *t } -> strict_value_eq;
                              }));

template <typename T>
concept output_iterator =
    forward_incrementable_iterator<T> && requires(T t) {
                                           { *t } -> reference_eq;
                                         };

template <typename T>
concept input_or_output_iterator = input_iterator<T> || output_iterator<T>;

template <typename T>
concept equality_comparable_iterator = equality_comparable<T, T>;

template <typename T>
concept forward_iterator =
    input_or_output_iterator<T> && equality_comparable_iterator<T>;

template <typename T>
concept backward_iterator =
    input_or_output_iterator<T> && backward_incrementable_iterator<T> &&
    equality_comparable_iterator<T>;

template <typename T>
concept bidirectional_iterator = forward_iterator<T> && backward_iterator<T>;

template <typename T>
concept random_iterator = bidirectional_iterator<T> && requires(T t) {
                                                         { t[0] };
                                                       };

template <typename T>
concept distanciable_iterator = substractable<T, T>;

template <typename C>
concept input_range = requires(C c) {
                        { c.begin() } -> input_iterator;
                      } && requires(C c) {
                             { c.end() } -> input_iterator;
                           };

template <typename C>
concept output_range = requires(C c) {
                         { c.begin() } -> output_iterator;
                       } && requires(C c) {
                              { c.end() } -> output_iterator;
                            };

template <typename C>
concept range = requires(C c) {
                  { c.begin() } -> input_or_output_iterator;
                } && requires(C c) {
                       { c.end() } -> input_or_output_iterator;
                     };

template <typename C>
concept forward_range = requires(C c) {
                          { c.begin() } -> forward_iterator;
                        } && requires(C c) {
                               { c.end() } -> forward_iterator;
                             };

template <typename C>
concept backward_range = requires(C c) {
                           { c.begin() } -> backward_iterator;
                         } && requires(C c) {
                                { c.end() } -> backward_iterator;
                              };

template <typename C>
concept bidirectional_range = requires(C c) {
                                { c.begin() } -> bidirectional_iterator;
                              } && requires(C c) {
                                     { c.end() } -> bidirectional_iterator;
                                   };

template <typename C>
concept random_range = requires(C c) {
                         { c.begin() } -> random_iterator;
                       } && requires(C c) {
                              { c.end() } -> random_iterator;
                            };

template <typename C>
concept range_or_bounded_array = range<C> || bounded_array_like<C>;

template <range R>
struct range_traits {
  using value_type = rm_cvref<decltype(*(R{}.begin()))>;
  using reference = value_type &;
  using pointer = value_type *;
  using const_reference = const value_type &;
};

template <range R>
using range_value_type = typename range_traits<R>::value_type;

template <range R>
using range_reference = typename range_traits<R>::reference;

template <range R>
using range_pointer = typename range_traits<R>::pointer;

template <range R>
using range_const_reference = typename range_traits<R>::const_reference;

template <typename C, typename T>
concept push_container = requires(C c, T t) { c.push(t); };

template <typename C, typename T>
concept pop_container = requires(C c) {
                          { c.pop() } -> convertible_to<T &>;
                        } || requires(C c) {
                               { c.pop() } -> convertible_to<T>;
                             } || requires(C c) {
                                    { c.pop() } -> convertible_to<const T &>;
                                  } || requires(C c) {
                                         { c.pop() } -> convertible_to<T &&>;
                                       } || requires(C c) {
                                              {
                                                c.pop()
                                                } -> convertible_to<const T &&>;
                                            };

template <typename C, typename T>
concept container = push_container<C, T> && pop_container<C, T>;

template <range R>
constexpr auto begin(R &&r) {
  return relay<R>(r).begin();
}

template <range R>
constexpr auto end(R &&r) {
  return relay<R>(r).end();
}

template <typename T, size_t N>
constexpr auto begin(T (&ar)[N]) {
  return ar;
}

template <typename T, size_t N>
constexpr auto end(T (&ar)[N]) {
  return ar + N;
}

template <input_or_output_iterator I>
class view {
 private:
  I _begin;
  I _end;

 public:
  constexpr ~view() = default;
  constexpr view() = default;
  constexpr view(I b, I e) : _begin(b), _end(e) {}

  template <input_range R>
  constexpr view(R &&r) : view(stew::begin(r), stew::end(r)) {}

  constexpr view(const view &) = default;
  constexpr view(view &&) = default;

  constexpr view &operator=(const view &) = default;
  constexpr view &operator=(view &&) = default;

 public:
  constexpr auto begin() { return _begin; }

  constexpr auto end() { return _end; }

  constexpr auto begin() const { return _begin; }

  constexpr auto end() const { return _end; }

 public:
  constexpr auto size() const
    requires distanciable_iterator<I>
  {
    return _begin == nullptr ? 0 : _end - _begin;
  }

  constexpr auto empty() const
    requires distanciable_iterator<I>
  {
    return size() == 0;
  }

  constexpr const auto &operator[](size_t i) const
    requires random_iterator<I>
  {
    return (_begin[i]);
  }
};

template <input_range R>
view(R &&r) -> view<decltype(begin(relay<R>(r)))>;

template <input_or_output_iterator I>
view(I b, I e) -> view<I>;

template <input_range R1, input_range R2>
constexpr bool equals(const R1 &r1, const R2 &r2) {
  auto b1 = begin(r1);
  auto b2 = begin(r2);

  auto e1 = end(r1);
  auto e2 = end(r2);

  while (b1 != e1 && b2 != e2 && *b1 == *b2) {
    ++b1;
    ++b2;
  }

  return b1 == e1 && b2 == e2;
}

template <input_range R1, input_range R2>
constexpr bool operator==(const R1 &r1, const R2 &r2) {
  return equals(r1, r2);
}

template <input_range R1, input_range R2>
constexpr bool operator!=(const R1 &r1, const R2 &r2) {
  return !equals(r1, r2);
}

template <input_range R1, input_range R2>
constexpr bool starts_with(const R1 &r1, const R2 &r2) {
  auto b1 = begin(r1);
  auto b2 = begin(r2);

  auto e1 = end(r1);
  auto e2 = end(r2);

  while (b1 != e1 && b2 != e2 && *b1 == *b2) {
    ++b1;
    ++b2;
  }

  return b2 == e2;
}

template <input_range R, typename T>
constexpr bool starts_with(const R &r, const T &t) {
  auto b = begin(r);
  return b != end(r) && *b == t;
}

template <input_range R, typename T>
constexpr auto find(R &r, const T &t) {
  auto b = begin(r);
  auto e = end(r);

  while (b != e && *b != t) {
    ++b;
  }

  return b;
}

template <input_range R1, range R2>
constexpr auto find(R1 &r1, const R2 &r2) {
  auto b1 = begin(r1);
  auto e1 = end(r1);

  while (b1 != e1 && !starts_with(view(b1, e1), r2)) {
    ++b1;
  }

  return b1;
}

template <input_range R, predicate<range_const_reference<R>> P>
constexpr auto find(R &r, P &&pred) {
  auto b = begin(r);
  auto e = end(r);

  while (b != e && !relay<P>(pred)(*b)) {
    ++b;
  }

  return b;
}

template <input_range R, equality_comparable<range_value_type<R>> T>
constexpr size_t count(const R &r, T &&t) {
  size_t c = 0;

  for (const auto &i : r) {
    if (i == relay<T>(t)) {
      ++c;
    }
  }

  return c;
}

template <input_range R0, input_range R1>
constexpr size_t count(const R0 &r0, const R1 &r1) {
  size_t c = 0;

  auto v0 = view(r0);
  auto v1 = view(r1);

  while (!v0.empty()) {
    if (starts_with(v0, v1)) {
      v0 = view(v0.begin() + v1.size(), v0.end());
      ++c;
    } else {
      v0 = view(v0.begin() + 1, v0.end());
    }
  }

  return c;
}

template <input_range R, predicate<range_const_reference<R>> P>
constexpr size_t count(const R &r, P &&pred) {
  size_t c = 0;

  for (const auto &i : r) {
    if (relay<P>(pred)(i)) {
      ++c;
    }
  }

  return c;
}

template <input_range R, typename T>
constexpr bool contains(const R &r, const T &t) {
  return find(r, t) != end(r);
}

template <input_range R1, input_range R2>
constexpr bool contains(const R1 &r1, const R2 &r2) {
  return find(r1, r2) != end(r1);
}

template <input_range R1, predicate<range_const_reference<R1>> P>
constexpr bool all_of(const R1 &r, P &&p) {
  for (const auto &i : r) {
    if (!p(i)) {
      return false;
    }
  }

  return true;
}

template <input_range R1, predicate<range_const_reference<R1>> P>
constexpr bool any_of(const R1 &r, P &&p) {
  for (const auto &i : r) {
    if (p(i)) {
      return true;
    }
  }

  return false;
}

template <input_range R1, predicate<range_const_reference<R1>> P>
constexpr bool none_of(const R1 &r, P &&p) {
  for (const auto &i : r) {
    if (p(i)) {
      return false;
    }
  }

  return true;
}

template <input_range R, output_iterator I>
constexpr void copy(R &&r, I i) {
  auto b = begin(relay<R>(r));
  auto e = end(relay<R>(r));

  while (b != e) {
    *i = *b;
    ++i;
    ++b;
  }
}

}  // namespace stew

#endif
