#ifndef __steward_container_hpp__
#define __steward_container_hpp__

#include <stew/iterator.hpp>

namespace stew {

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

template <range R>
constexpr size_t size(R &&r)
  requires distanciable_iterator<decltype(r.begin())>
{
  return begin(r) - end(r);
}

}  // namespace stew

#endif
