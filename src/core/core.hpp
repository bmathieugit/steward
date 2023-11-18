#ifndef __n_utils_hpp__
#define __n_utils_hpp__

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

template <typename From, typename To>
concept convertible_to =
    (same_as<From, void> && same_as<To, void>) ||
    requires { static_cast<rm_cref<To>>(*((rm_cref<From>*)nullptr)); };

template <typename T>
concept character = same_as<T, char> or same_as<T, wchar_t>;

template <typename T>
concept signed_integral = same_as<T, short> or same_as<T, int> or
                          same_as<T, long> or same_as<T, long long>;

template <typename T>
concept unsigned_integral =
    same_as<T, unsigned short> or same_as<T, unsigned int> or
    same_as<T, unsigned long> or same_as<T, unsigned long long>;

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

template <bool test, auto t, auto f>
constexpr auto tern = test ? t : f;

template <typename T>
concept integral = signed_integral<T> or unsigned_integral<T> or character<T>;

template <typename T>
concept default_constructible = requires { T(); };

using uchar = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long;
using ullong = unsigned long long;

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

template <typename T>
concept byteable = requires(T t) {
  { bytes(t) } -> same_as<byte_t[sizeof(size_t) + 1]>;
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

template <typename T, typename U>
concept has_equals_operator = requires(const T& t, const U& u) { t == u; };

template <typename T, typename U>
concept has_diffs_operator = requires(const T& t, const U& u) { t != u; };

template <typename P, typename T>
concept predicate = requires(P p, T t) {
  { p(t) } -> same_as<bool>;
};

template <typename T>
concept collection_context = requires {
  typename T::type;
  typename T::position;
};

template <typename T1, typename T2>
concept same_type = same_as<typename T1::type, typename T2::type>;

template <typename C>
concept collection =
    collection_context<C> and
    requires(C c, const C cc, typename C::type t, typename C::position p) {
      { cc.has(p) } -> same_as<bool>;
      { cc.at(p) } -> same_as_declined<typename C::type>;
      { cc.empty() } -> same_as<bool>;
      { cc.len() } -> same_as<size_t>;
      { c.add(t, p) } -> same_as<bool>;
      { c.modify(t, p) } -> same_as<bool>;
      { c.remove(p) } -> same_as<bool>;
      { c.clear() } -> same_as<void>;
    };

template <typename C>
concept char_collection = collection<C> and character<typename C::type>;

template <typename I>
concept iterator = requires(I i) {
  typename I::type;
  { i.has() } -> same_as<bool>;
  { i.next() } -> same_as_declined<typename I::type>;
};

template <typename O>
concept oterator = requires(O o, typename O::type c) {
  typename O::type;
  { o.add(c) } -> same_as<bool>;
};

template <typename C>
concept char_iterator = character<typename C::type> and iterator<C>;

template <typename C>
concept char_oterator = character<typename C::type> and oterator<C>;

template <collection C>
class index_based_iterator {
 public:
  using type = typename C::type;

 private:
  const C& _col;
  size_t _index;

 public:
  constexpr index_based_iterator(const C& c) : _col(c), _index(0) {}
  constexpr bool has() { return _index != _col.len(); }
  constexpr const type& next() { return _col.at(_index++); }
};

template <collection C>
class index_based_riterator {
 public:
  using type = typename C::type;

 private:
  const C& _col;
  size_t _index;

 public:
  constexpr index_based_riterator(const C& c) : _col(c), _index(_col.len()) {}
  constexpr bool has() { return _index != 0; }
  constexpr const type& next() { return _col.at(--_index); }
};

template <collection C>
class index_based_oterator {
 public:
  using type = typename C::type;

 private:
  C& _col;

 public:
  constexpr index_based_oterator(C& c) : _col(c) {}
  constexpr bool add(const type& t) { return _col.add(t); }
  constexpr bool add(type&& t) { return _col.add(move(t)); }
};

constexpr size_t prand(size_t a, size_t b, size_t seed) {
  seed = (a * seed + b) & max_of<unsigned>;
  return static_cast<size_t>(seed);
}

constexpr size_t rand(size_t seed) {
  return prand(1103515245, 12345, seed);
}

struct random_index_input_stream {
 public:
  using type = size_t;

 private:
  size_t _seed = 0;
  size_t _max = 0;

 public:
  constexpr random_index_input_stream(size_t seed, size_t max)
      : _seed(seed), _max(max) {}

 public:
  constexpr bool has() { return true; }
  constexpr size_t next() { return (_seed = rand(_seed)) % _max; }
};
#endif
