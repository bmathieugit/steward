#ifndef __steward_meta_hpp__
#define __steward_meta_hpp__

namespace stew {
using size_t = unsigned long long;
using byte_t = unsigned char;
using nullptr_t = decltype(nullptr);
}  // namespace stew

namespace stew {
//--------------------------------
//
// Metaprogramming
//
//--------------------------------

template <typename C, typename T>
constexpr C cast(T &&t) noexcept {
  return static_cast<C>(t);
}

template <typename T>
constexpr T fakeval();

template <typename T>
struct struct_type {
  using type = T;
};

template <typename T>
using type = typename T::type;

//--------------------------------
//
// Meta : add and remove on T
//
//--------------------------------

namespace impl {
template <typename T>
struct rm_ref : struct_type<T> {};

template <typename T>
struct rm_ref<T &> : struct_type<T> {};

template <typename T>
struct rm_ref<T &&> : struct_type<T> {};

template <typename T>
struct rm_const : struct_type<T> {};

template <typename T>
struct rm_const<const T> : struct_type<T> {};

template <typename T>
struct rm_volatile : struct_type<T> {};

template <typename T>
struct rm_volatile<volatile T> : struct_type<T> {};

template <typename T>
struct rm_array : struct_type<T> {};

template <typename T>
struct rm_array<T[]> : struct_type<T> {};

template <typename T, size_t N>
struct rm_array<T[N]> : struct_type<T> {};

template <typename T>
struct rm_pointer : struct_type<T> {};

template <typename T>
struct rm_pointer<T *> : struct_type<T> {};
}  // namespace impl

template <typename T>
using add_const = const T;

template <typename T>
using rm_const = type<impl::rm_const<T>>;

template <typename T>
using add_volatile = volatile T;

template <typename T>
using rm_volatile = type<impl::rm_volatile<T>>;

template <typename T>
using rm_cv = rm_const<rm_volatile<T>>;

template <typename T>
using add_lref = T &;

template <typename T>
using add_rref = T &&;

template <typename T>
using rm_ref = type<impl::rm_ref<T>>;

template <typename T>
using rm_cvref = rm_const<rm_volatile<rm_ref<T>>>;

template <typename T>
using add_array = T[];

template <typename T>
using rm_array = type<impl::rm_array<T>>;

template <typename T>
using add_pointer = T *;

template <typename T>
using rm_pointer = type<impl::rm_pointer<T>>;

template <typename T>
using strictly = rm_cvref<T>;

//-----------------------------------
//
//  Meta : metaprogramming
//
//-----------------------------------

namespace impl {
template <bool B, typename T0, typename T1>
struct if_;

template <typename T0, typename T1>
struct if_<true, T0, T1> : struct_type<T0> {};

template <typename T0, typename T1>
struct if_<false, T0, T1> : struct_type<T1> {};
}  // namespace impl

template <bool B, typename T0, typename T1>
using if_ = type<impl::if_<B, T0, T1>>;

//-----------------------------------
//
// Meta : generals concepts
//
//-----------------------------------

namespace impl {
template <typename T, typename U>
constexpr bool same_as = false;

template <typename T>
constexpr bool same_as<T, T> = true;
}  // namespace impl

template <typename T, typename U>
concept same_as = impl::same_as<T, U>;

template <typename T, typename... U>
concept same_one_of = (same_as<T, U> || ...);

template <typename From, typename To>
concept convertible_to = (same_as<From, void> && same_as<To, void>) ||
                         requires {
                           static_cast<rm_cvref<To>>(
                               *((rm_cvref<From> *)nullptr));
                         };

template <typename T, typename U>
concept not_convertible_to = (!convertible_to<T, U>);

template <typename T, typename U>
concept like = convertible_to<T, U>;

template <typename T>
concept strict_value_eq = same_as<T, rm_ref<T>>;

template <typename T>
concept reference_eq = same_as<T, rm_cvref<T> &>;

template <typename T>
concept const_reference_eq = same_as<T, const rm_cvref<T> &>;

template <typename T>
concept double_reference_eq = same_as<T, rm_cvref<T> &&>;

template <typename T>
concept const_double_reference_eq = same_as<T, const rm_cvref<T> &&>;

template <typename T>
concept pointer_eq = same_as<T, rm_pointer<T> *>;

template <typename T>
concept not_strict_value_eq = (!strict_value_eq<T>);

template <typename T>
concept not_reference_eq = (!reference_eq<T>);

template <typename T>
concept not_const_reference_eq = (!const_reference_eq<T>);

template <typename T>
concept not_double_reference_eq = (!double_reference_eq<T>);

template <typename T>
concept not_const_double_reference_eq = (!const_double_reference_eq<T>);

template <typename T>
concept not_pointer_eq = (!pointer_eq<T>);

template <typename T, typename R, typename... A>
concept callable = requires(T t, A &&...a) {
                     { t(a...) } -> convertible_to<R>;
                   };

template <typename P, typename... A>
concept predicate = callable<P, bool, A...>;

template <typename T, size_t N>
concept size_more_than = (sizeof(T) > N);

template <typename T, size_t N>
concept size_lesseq_than = (sizeof(T) <= N);

//-------------------------------------
//
// Meta : has operators concepts
//
//-------------------------------------

template <typename T>
concept notable = requires(T t) { !t; };

template <typename T>
concept prefix_incrementable = requires(T t) { ++t; };

template <typename T>
concept suffix_incrementable = requires(T t) { t++; };

template <typename T>
concept incrementable = prefix_incrementable<T> || suffix_incrementable<T>;

template <typename T>
concept prefix_decrementable = requires(T t) { ++t; };

template <typename T>
concept suffix_decrementable = requires(T t) { t++; };

template <typename T>
concept decrementable = prefix_decrementable<T> || suffix_decrementable<T>;

template <typename T>
concept indirectlyable = requires(T t) { *t; };

template <typename T>
concept addressable = requires(T t) { &t; };

template <typename T, typename U = T>
concept equality_comparable = requires(T t, U u) { t == u; };

template <typename T, typename U = T>
concept less_comparable = requires(T t, U u) { t < u; };

template <typename T, typename U = T>
concept greater_comparable = requires(T t, U u) { t > u; };

template <typename T, typename U = T>
concept lesseq_comparable = requires(T t, U u) { t <= u; };

template <typename T, typename U = T>
concept greatereq_comparable = requires(T t, U u) { t >= u; };

template <typename T, typename U = T>
concept and_comparable = requires(T t, U u) { t and u; };

template <typename T, typename U = T>
concept or_comparable = requires(T t, U u) { t or u; };

template <typename T, typename U = T>
concept addable = requires(T t, U u) { t + u; };

template <typename T, typename U = T>
concept substractable = requires(T t, U u) { t - u; };

template <typename T, typename U = T>
concept multiplyable = requires(T t, U u) { t *u; };

template <typename T, typename U = T>
concept dividable = requires(T t, U u) { t / u; };

template <typename T, typename U = T>
concept modulable = requires(T t, U u) { t % u; };

//------------------------------------
//
// Meta : type familly
//
//------------------------------------

template <typename T>
concept character = same_one_of<T, char, wchar_t>;

template <typename T>
concept signed_integral = same_one_of<T, short, int, long, long long>;

template <typename T>
concept unsigned_integral = same_one_of<T, unsigned short, unsigned int,
                                        unsigned long, unsigned long long>;

template <typename T>
concept integral = signed_integral<T> || unsigned_integral<T>;

template <typename T>
concept plain_integral = integral<T> || character<T>;

template <typename T>
concept floating_point = same_one_of<T, float, double, long double>;

template <typename T>
concept native_number =
    integral<T> || character<T> || floating_point<T> || same_as<T, bool>;

namespace impl {
template <typename T>
constexpr bool native_array_like = false;

template <typename T>
constexpr bool native_array_like<T[]> = true;

template <typename T, size_t N>
constexpr bool native_array_like<T[N]> = true;

template <typename T, size_t N>
constexpr bool native_array_like<T (&)[N]> = true;
}  // namespace impl

template <typename T>
concept native_array_like = impl::native_array_like<T>;

namespace impl {
template <typename T>
constexpr bool bounded_array_like = false;

template <typename T, size_t N>
constexpr bool bounded_array_like<T[N]> = true;

template <typename T, size_t N>
constexpr bool bounded_array_like<T (&)[N]> = true;
}  // namespace impl

template <typename T>
concept bounded_array_like = impl::bounded_array_like<T>;

namespace impl {
template <typename T>
constexpr bool unbounded_array_like = false;

template <typename T>
constexpr bool unbounded_array_like<T[]> = true;

template <typename T>
constexpr bool unbounded_array_like<T (&)[]> = true;
}  // namespace impl

template <typename T>
concept unbounded_array_like = impl::unbounded_array_like<T>;

template <typename D, typename B>
constexpr bool derived_from =
    requires { dynamic_cast<B *>(static_cast<D *>(nullptr)); } &&
    requires { dynamic_cast<B &>(*static_cast<D *>(nullptr)); };

//-----------------------------------
//
// Meta : integral validation
//
//-----------------------------------

template <auto I0, auto I1>
constexpr bool value_more_than = (I0 > I1);

template <auto I0, auto I1>
constexpr bool value_moreeq_than = (I0 >= I1);

template <auto I0, auto I1>
constexpr bool value_less_than = (I0 < I1);

template <auto I0, auto I1>
constexpr bool value_lesseq_than = (I0 <= I1);

template <auto I0, auto I1>
constexpr bool value_eq_to = (I0 == I1);

template <auto I0, auto I1>
constexpr bool value_noteq_to = (I0 != I1);

}  // namespace stew

#endif
