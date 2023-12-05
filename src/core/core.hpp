#ifndef __n_utils_hpp__
#define __n_utils_hpp__

template <typename T>
constexpr bool __is_reference = false;

template <typename T>
constexpr bool __is_reference<T&> = true;

template <typename T>
concept is_reference = __is_reference<T>;

template <typename T>
constexpr bool __is_const_reference = false;

template <typename T>
constexpr bool __is_const_reference<const T&> = true;

template <typename T>
concept is_const_reference = __is_const_reference<T>;

template <typename T>
constexpr bool __is_move_reference = false;

template <typename T>
constexpr bool __is_move_reference<T&&> = true;

template <typename T>
concept is_move_reference = __is_move_reference<T>;

template <typename T>
constexpr bool __is_const_move_reference = false;

template <typename T>
constexpr bool __is_const_move_reference<const T&&> = true;

template <typename T>
concept is_const_move_reference = __is_const_move_reference<T>;

template <typename T>
concept is_any_reference = is_reference<T> or is_const_reference<T> or
                           is_move_reference<T> or is_const_move_reference<T>;

template <typename T>
concept is_readonly_reference =
    is_const_reference<T> or is_const_move_reference<T>;

// Est ce que un move reference est writeable ?
template <typename T>
concept is_readwrite_reference = is_reference<T> or is_move_reference<T>;

template <typename T>
struct __rm_ref {
  using type = T;
};

template <typename T>
struct __rm_ref<T&> {
  using type = T;
};

template <typename T>
struct __rm_ref<T&&> {
  using type = T;
};

template <typename T>
using rm_ref = typename __rm_ref<T>::type;

template <typename T>
struct __rm_const {
  using type = T;
};

template <typename T>
struct __rm_const<const T> {
  using type = T;
};

template <typename T>
using rm_const = typename __rm_const<T>::type;

template <typename T>
using rm_cref = rm_const<rm_ref<T>>;

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

template <typename T, typename... TN>
concept same_all = (same_as<T, TN> and ... and true);

template <typename From, typename To>
concept convertible_to =
    (same_as<From, void> && same_as<To, void>) ||
    requires { static_cast<rm_cref<To>>(*((rm_cref<From>*)nullptr)); };

template <typename T>
concept default_constructible = requires { T(); };

template <bool test, typename T, typename U>
struct __if_ {
  using type = U;
};

template <typename T, typename U>
struct __if_<true, T, U> {
  using type = T;
};

template <bool test, typename T, typename U>
using if_ = typename __if_<test, T, U>::type;

template <auto v, typename T>
struct switch_case_ {
  using type = T;
  static constexpr auto value = v;
};

template <auto v, typename... C>
struct __switch_ {
  using type = void;
};

template <auto v, typename C0, typename... CN>
struct __switch_<v, C0, CN...> {
  using type = if_<(v == C0::value),
                   typename C0::type,
                   typename __switch_<v, CN...>::type>;
};

template <auto v, typename... C>
using switch_ = typename __switch_<v, C...>::type;

template <typename KEY, typename VAL>
struct map_pair_ {
  using K = KEY;
  using V = VAL;
};

template <typename K, typename... P>
struct __map_ {
  using type = void;
};

template <typename K, typename P0, typename... PN>
struct __map_<K, P0, PN...> {
  using type = if_<(same_as<K, typename P0::K>),
                   typename P0::V,
                   typename __map_<K, PN...>::type>;
};

template <typename K, typename... P>
using map_ = typename __map_<K, P...>::type;

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;
using ullong = unsigned long long;

template <typename T>
concept character = contains<T, char, wchar_t>;

template <typename T>
concept signed_integral = contains<T, short, int, long, long long>;

template <typename T>
concept unsigned_integral = contains<T, ushort, uint, ulong, ullong>;

template <typename T>
concept integral = signed_integral<T> or unsigned_integral<T> or character<T>;

template <unsigned_integral U>
using signed_eq = map_<U,
                       map_pair_<uchar, char>,
                       map_pair_<ushort, short>,
                       map_pair_<uint, int>,
                       map_pair_<ulong, long>,
                       map_pair_<ullong, long long>>;

template <signed_integral S>
using unsigned_eq = map_<S,
                         map_pair_<char, uchar>,
                         map_pair_<short, ushort>,
                         map_pair_<int, uint>,
                         map_pair_<long, ulong>,
                         map_pair_<long long, ullong>>;

template <typename I>
constexpr I max_of;

template <unsigned_integral I>
constexpr I max_of<I> = static_cast<I>(~0);

template <signed_integral I>
constexpr I max_of<I> = static_cast<I>(max_of<unsigned_eq<I>> / 2);

template <typename I>
constexpr I min_of;

template <unsigned_integral I>
constexpr I min_of<I> = 0;

template <signed_integral I>
constexpr I min_of<I> = (-1 * (max_of<unsigned_eq<I>> / 2)) - 1;

template <decltype(sizeof(int)) s>
using iof = switch_<s / 8,
                    switch_case_<sizeof(char), char>,
                    switch_case_<sizeof(short), short>,
                    switch_case_<sizeof(int), int>,
                    switch_case_<sizeof(long), long>,
                    switch_case_<sizeof(long long), long long>>;

template <decltype(sizeof(int)) s>
using uof = switch_<s / 8,
                    switch_case_<sizeof(uchar), uchar>,
                    switch_case_<sizeof(ushort), ushort>,
                    switch_case_<sizeof(uint), uint>,
                    switch_case_<sizeof(ulong), ulong>,
                    switch_case_<sizeof(ullong), ullong>>;

using i8 = iof<8>;
using i16 = iof<16>;
using i32 = iof<32>;
using i64 = iof<64>;
using i128 = iof<128>;

using u8 = uof<8>;
using u16 = uof<16>;
using u32 = uof<32>;
using u64 = uof<64>;
using u128 = uof<128>;

using byte_t = u8;
using size_t = decltype(sizeof(int));

template <typename T, typename U>
concept has_equals_operator = requires(const T& t, const U& u) { t == u; };

template <typename T, typename U>
concept has_diffs_operator = requires(const T& t, const U& u) { t != u; };

template <typename U, typename T, typename O>
concept unary = requires(U u, T t) {
  { u(t) } -> same_as<O>;
};

template <typename P, typename T>
concept predicate = unary<P, T, bool>;

template <typename I>
concept __iterator_has_bool_equals_operator = requires(I i) {
  { i == i } -> same_as<bool>;
};

template <typename I>
concept __iterator_has_bool_diffs_operator = requires(I i) {
  { i != i } -> same_as<bool>;
};

template <typename I>
constexpr bool ___iterator_is_pointer_type = false;

template <typename I>
constexpr bool ___iterator_is_pointer_type<I*> = true;

template <typename I>
concept __iterator_is_pointer_type = ___iterator_is_pointer_type<I>;

template <typename I>
struct __iterator_type {
  using type = typename I::type;
};

template <typename I>
struct __iterator_type<I*> {
  using type = I;
};

template <typename I>
using iterator_type = typename __iterator_type<I>::type;

template <typename I>
concept __iterator_has_preincrement_operator = requires(I i) {
  { ++i } -> same_as<I&>;
};

template <typename I>
concept __iterator_has_postincrement_operator = requires(I i) {
  { i++ } -> same_as<I>;
};

template <typename I, typename T>
concept __iterator_has_dereference_operator = requires(I i) {
  { *i } -> same_as_declined<T>;
};

template <typename I>
concept iterator = __iterator_has_bool_diffs_operator<I> and
                   __iterator_has_bool_equals_operator<I> and
                   __iterator_has_preincrement_operator<I> and
                   __iterator_has_postincrement_operator<I> and
                   __iterator_has_dereference_operator<I, iterator_type<I>>;

template <typename I, typename T>
concept __iterator_can_assign_operator = requires(I i, T t) {
  { *i = t };
};

template <typename I>
concept oterator = __iterator_has_bool_diffs_operator<I> and
                   __iterator_has_bool_equals_operator<I> and
                   __iterator_has_preincrement_operator<I> and
                   __iterator_has_postincrement_operator<I> and
                   __iterator_can_assign_operator<I, iterator_type<I>>;

template <typename T>
concept distanciable = requires(T t) {
  { t - t } -> same_as<size_t>;
};

template <typename T>
constexpr rm_ref<T>&& move(T&& t) {
  return static_cast<rm_ref<T>&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& relay(rm_ref<T>&& t) noexcept {
  return static_cast<T&&>(t);
}

template <typename T>
void* operator new(size_t, T* ptr) {
  return ptr;
}
#endif
