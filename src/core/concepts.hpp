#ifndef __stew_core_concepts_hpp__
#define __stew_core_concepts_hpp__

#include <core/utils.hpp>

template <typename T, typename U>
constexpr bool __same_as = false;

template <typename T>
constexpr bool __same_as<T, T> = true;

template <typename T, typename U>
concept same_as = __same_as<T, U>;

template <typename U, typename T0, typename... T>
concept contains = (same_as<U, T> or ...) or same_as<U, T0>;

template <typename T, typename U>
concept same_as_basic = __same_as<rm_cref<T>, rm_cref<U>>;

template <typename T, typename U>
concept same_as_declined = contains<T,
                                    rm_cref<U>,
                                    rm_cref<U>&,
                                    rm_cref<U>&&,
                                    const rm_cref<U>,
                                    const rm_cref<U>&,
                                    const rm_cref<U>&&>;

template <typename T>
concept character = same_as<T, char> or same_as<T, wchar_t>;

template <typename T>
concept signed_integral = same_as<T, short> or same_as<T, int> or
                          same_as<T, long> or same_as<T, long long>;

template <typename T>
concept unsigned_integral =
    same_as<T, unsigned short> or same_as<T, unsigned int> or
    same_as<T, unsigned long> or same_as<T, unsigned long long>;

template <typename T>
concept default_constructible = requires { T(); };

template <typename T, typename U>
concept has_equals_operator = requires(const T& t, const U& u) { t == u; };

template <typename T, typename U>
concept has_diffs_operator = requires(const T& t, const U& u) { t != u; };

template <typename T>
concept collection_context = requires {
  typename T::type;
  typename T::position;
};

template <typename C>
concept collection =
    collection_context<C> and
    requires(C c, const C cc, typename C::type t, typename C::position p) {
      { cc.has(p) } -> same_as<bool>;
      { cc.at(p) } -> same_as_declined<typename C::type>;
      { cc.empty() } -> same_as<bool>;
      { cc.len() } -> same_as<size_t>;
      { c.add(t) } -> same_as<bool>;
      { c.modify(p, t) } -> same_as<bool>;
      { c.remove(p) } -> same_as<bool>;
      { c.clear() } -> same_as<void>;
    };

template <typename I>
concept input_stream = requires { typename I::type; } and requires(I i) {
  { i.next() } -> same_as_declined<typename I::type>;
  { i.has() } -> same_as<bool>;
};

template <typename O>
concept output_stream =
    requires { typename O::type; } and requires(O o, typename O::type c) {
      { o.add(c) } -> same_as<bool>;
    };

template <typename S>
concept char_input_stream = input_stream<S> and character<typename S::type>;

template <typename S>
concept char_output_stream = output_stream<S> and character<typename S::type>;

#endif