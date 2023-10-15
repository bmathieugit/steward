#ifndef __stew_core_concepts_hpp__
#define __stew_core_concepts_hpp__

#include <core/utils.hpp>
/**
 * @brief Concept to check if two types are the same.
 *
 * The 'same_as' concept checks if two types, 'T' and 'U', are the same.
 *
 * @tparam T The first type.
 * @tparam U The second type.
 */
template <typename T, typename U>
constexpr bool __same_as = false;

/**
 * @brief Specialization of 'same_as' concept for identical types.
 *
 * When 'T' and 'U' are the same types, this specialization sets the concept to
 * true.
 *
 * @tparam T The first type.
 */
template <typename T>
constexpr bool __same_as<T, T> = true;

/**
 * @brief Concept to check if two types are the same.
 *
 * The 'same_as' concept uses '__same_as' to determine if two types 'T' and 'U'
 * are identical.
 *
 * @tparam T The first type.
 * @tparam U The second type.
 */
template <typename T, typename U>
concept same_as = __same_as<T, U>;

/**
 * @brief Concept to check if a type is contained within a list of types.
 *
 * The 'contains' concept checks if a type 'U' is contained within a list of
 * types 'T0' and 'T'.
 *
 * @tparam U The type to be checked for containment.
 * @tparam T0 The first type in the list.
 * @tparam T A parameter pack representing the list of types.
 */
template <typename U, typename T0, typename... T>
concept contains = (same_as<U, T> or ...) or same_as<U, T0>;

/**
 * @brief Concept to check if two types are the same, considering reference and
 * cv-qualifiers.
 *
 * The 'same_as_basic' concept checks if two types 'T' and 'U' are the same
 * after removing reference and cv-qualifiers.
 *
 * @tparam T The first type.
 * @tparam U The second type.
 */
template <typename T, typename U>
concept same_as_basic = __same_as<rm_cref<T>, rm_cref<U>>;

/**
 * @brief Concept to check if a type is the same as several declined variations
 * of itself.
 *
 * The 'same_as_declined' concept checks if a type 'T' is the same as various
 * declined variations of U.
 *
 * @tparam T The type to be checked.
 * @tparam U The type declined.
 */
template <typename T, typename U>
concept same_as_declined = contains<T,
                                    rm_cref<U>,
                                    rm_cref<U>&,
                                    rm_cref<U>&&,
                                    const rm_cref<U>,
                                    const rm_cref<U>&,
                                    const rm_cref<U>&&>;

/**
 * @brief Concept to check if a type is a character type (char or wchar_t).
 *
 * The 'character' concept checks if a type 'T' is the same as 'char' or
 * 'wchar_t'.
 *
 * @tparam T The type to be checked.
 */
template <typename T>
concept character = same_as<T, char> or same_as<T, wchar_t>;

/**
 * @brief Concept to check if a type is a signed integral type (short, int,
 * long, or long long).
 *
 * The 'signed_integral' concept checks if a type 'T' is the same as 'short',
 * 'int', 'long', or 'long long'.
 *
 * @tparam T The type to be checked.
 */
template <typename T>
concept signed_integral = same_as<T, short> or same_as<T, int> or
                          same_as<T, long> or same_as<T, long long>;

/**
 * @brief Concept to check if a type is an unsigned integral type (unsigned
 * short, unsigned int, unsigned long, or unsigned long long).
 *
 * The 'unsigned_integral' concept checks if a type 'T' is the same as 'unsigned
 * short', 'unsigned int', 'unsigned long', or 'unsigned long long'.
 *
 * @tparam T The type to be checked.
 */
template <typename T>
concept unsigned_integral =
    same_as<T, unsigned short> or same_as<T, unsigned int> or
    same_as<T, unsigned long> or same_as<T, unsigned long long>;

/**
 * @brief Concept to check if a type is default-constructible.
 *
 * The 'default_constructible' concept checks if a type 'T' can be
 * default-constructed (supports 'T()').
 *
 * @tparam T The type to be checked.
 */
template <typename T>
concept default_constructible = requires { T(); };

/**
 * @brief Concept to check if two types support the '==' operator for equality
 * comparison.
 *
 * The 'has_equals_operator' concept checks if two types 'T' and 'U' support the
 * '==' operator for equality comparison.
 *
 * @tparam T The first type.
 * @tparam U The second type.
 */
template <typename T, typename U>
concept has_equals_operator = requires(const T& t, const U& u) { t == u; };

/**
 * @brief Concept to check if two types support the '!=' operator for inequality
 * comparison.
 *
 * The 'has_diffs_operator' concept checks if two types 'T' and 'U' support the
 * '!=' operator for inequality comparison.
 *
 * @tparam T The first type.
 * @tparam U The second type.
 */
template <typename T, typename U>
concept has_diffs_operator = requires(const T& t, const U& u) { t != u; };

/**
 * @brief Concept for the context of a collection.
 *
 * The collection_context concept specifies the requirements for the context of
 * a collection. It checks if the given type 'T' has the necessary type
 * definitions:
 * - 'typename T::type' to specify the type of elements in the collection.
 * - 'typename T::position' to specify the type for referencing positions within
 * the collection.
 *
 * @tparam T The type to be checked for collection context requirements.
 */
template <typename T>
concept collection_context = requires {
  typename T::type;
  typename T::position;
};

/**
 * @brief Concept for a collection.
 *
 * The collection concept defines the requirements for a collection type. To
 * satisfy this concept, a type 'C' must meet the following criteria:
 * - It must fulfill the 'collection_context' concept.
 * - It must provide the following functions with specific return types:
 *   - 'cc.has(p)' must return a boolean and indicates whether an element exists
 * at position 'p'.
 *   - 'cc.at(p)' returns an element of the same type as 'typename C::type'
 * located at position 'p'.
 *   - 'cc.empty()' must return a boolean indicating whether the collection is
 * empty.
 *   - 'cc.len()' must return a size_t representing the length of the
 * collection.
 *   - 'c.add(t)' returns a boolean and is used to add an element 't' to the
 * collection.
 *   - 'c.modify(p, t)' returns a boolean and is used to modify an element at
 * position 'p' with value 't'.
 *   - 'c.remove(p)' returns a boolean and is used to remove an element at
 * position 'p'.
 *   - 'c.clear()' returns void and is used to clear the entire collection.
 *
 * @tparam C The type to be checked for collection requirements.
 */
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

/**
 * @brief Concept for C++ iterators.
 *
 * The 'iterator' concept specifies the requirements for a C++ iterator. To
 * satisfy this concept, a type 'I' must meet the following criteria:
 * - It must fulfill the 'collection_context' concept.
 * - It must provide the following functions with specific return types:
 *   - 'ci.has()' returns a boolean and checks if there are more elements to
 * iterate.
 *   - 'i.next()' returns void and is used to advance to the next element in the
 * collection.
 *   - 'i.get()' returns an element of the same type as 'typename I::type' and
 * retrieves the current element from the collection.
 *   - 'ci.pos()' returns a value of the type 'typename I::position' to specify
 * the current position within the collection.
 *
 * @tparam I The type to be checked for iterator requirements.
 */
template <typename I>
concept iterator = collection_context<I> and requires(I i, const I ci) {
  { ci.has() } -> same_as<bool>;
  { i.next() } -> same_as<void>;
  { i.get() } -> same_as_declined<typename I::type>;
  { ci.pos() } -> same_as<typename I::position>;
};

/**
 * @brief Generic input concept for streams.
 *
 * This concept checks if a given type, I, meets the requirements of a generic
 * input stream. To satisfy this concept, type I must have a 'type' type
 * definition and two functions:
 * - i.next(): which returns a value of the same type as 'typename I::type'.
 * - i.has(): which returns a boolean indicating whether the input stream still
 * has data.
 */
template <typename I>
concept input_stream = requires { typename I::type; } and requires(I i) {
  { i.next() } -> same_as_declined<typename I::type>;
  { i.has() } -> same_as<bool>;
};

/**
 * @brief Generic output concept for streams.
 *
 * This concept checks if a given type, O, meets the requirements of a generic
 * output stream. To satisfy this concept, type O must have a 'type' type
 * definition and a function:
 * - o.add(c): which takes an argument of the same type as 'typename O::type'
 * and returns a boolean indicating the success of the addition.
 */
template <typename O>
concept output_stream =
    requires { typename O::type; } and requires(O o, typename O::type c) {
      { o.add(c) } -> same_as<bool>;
    };

/**
 * @brief Concept for character iterators.
 *
 * The 'char_iterator' concept specifies the requirements for a character
 * iterator. To satisfy this concept, a type 'I' must meet the following
 * criteria:
 * - It must fulfill the 'iterator' concept.
 * - The element type of the iterator, 'typename I::type', must satisfy the
 * 'character' concept.
 *
 * @tparam I The type to be checked for character iterator requirements.
 */
template <typename I>
concept char_iterator = iterator<I> and character<typename I::type>;

/**
 * @brief Concept for character input streams.
 *
 * The 'char_input_stream' concept defines the requirements for a character
 * input stream. To satisfy this concept, a type 'S' must meet the following
 * criteria:
 * - It must fulfill the 'input_stream' concept.
 * - The element type of the input stream, 'typename S::type', must satisfy the
 * 'character' concept.
 *
 * @tparam S The type to be checked for character input stream requirements.
 */
template <typename S>
concept char_input_stream = input_stream<S> and character<typename S::type>;

/**
 * @brief Concept for character output streams.
 *
 * The 'char_output_stream' concept outlines the requirements for a character
 * output stream. To satisfy this concept, a type 'S' must meet the following
 * criteria:
 * - It must fulfill the 'output_stream' concept.
 * - The element type of the output stream, 'typename S::type', must satisfy the
 * 'character' concept.
 *
 * @tparam S The type to be checked for character output stream requirements.
 */
template <typename S>
concept char_output_stream = output_stream<S> and character<typename S::type>;

#endif