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
concept integral = signed_integral<T> or unsigned_integral<T> or character<T>;

template <typename T>
concept default_constructible = requires { T(); };

// https://syntaxfix.com/question/49873/min-and-max-value-of-data-type-in-c
template <typename T>
constinit const T max_of =
    (((T)(~0LLU) > (T)((1LLU << ((sizeof(T) << 3) - 1)) - 1LLU))
         ? (long long unsigned int)(T)(~0LLU)
         : (long long unsigned int)(T)((1LLU << ((sizeof(T) << 3) - 1)) -
                                       1LLU));

// https://syntaxfix.com/question/49873/min-and-max-value-of-data-type-in-c
template <typename T>
constinit const T min_of =
    (((T)(1LLU << ((sizeof(T) << 3) - 1)) < (T)1)
         ? (long long int)((~0LLU) - ((1LLU << ((sizeof(T) << 3) - 1)) - 1LLU))
         : 0LL);

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

template <decltype(sizeof(int)) s>
using iof =
    if_<(sizeof(char) == s / 8),
        char,
        if_<(sizeof(short) == s / 8),
            short,
            if_<(sizeof(int) == s / 8),
                int,
                if_<(sizeof(long) == s / 8),
                    long,
                    if_<(sizeof(long long) == s / 8), long long, void>>>>>;

template <decltype(sizeof(int)) s>
using uof = if_<(sizeof(unsigned char) == s / 8),
                unsigned char,
                if_<(sizeof(unsigned short) == s / 8),
                    unsigned short,
                    if_<(sizeof(unsigned int) == s / 8),
                        unsigned int,
                        if_<(sizeof(unsigned long) == s / 8),
                            unsigned long,
                            if_<(sizeof(unsigned long long) == s / 8),
                                unsigned long long,
                                void>>>>>;

using u8 = uof<8>;
using u16 = uof<16>;
using u32 = uof<32>;
using u64 = uof<64>;
using u128 = uof<128>;

using i8 = iof<8>;
using i16 = iof<16>;
using i32 = iof<32>;
using i64 = iof<64>;
using i128 = iof<128>;

using byte_t = u8;
using size_t = if_<(not same_as<void, u64>),
                   u64,
                   if_<(not same_as<void, u32>),
                       u32,
                       if_<(not same_as<void, u16>), u16, void>>>;

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

template <typename T1, typename T2>
concept same_position = same_as<typename T1::position, typename T2::position>;

template <typename O>
concept ordinal = requires { typename O::position; } and requires(O o) {
  { o.has() } -> same_as<bool>;
  { o.next() } -> same_as_declined<typename O::position>;
};

template <typename C>
concept collection =
    collection_context<C> and
    requires(C c, const C cc, typename C::type t, typename C::position p) {
      { cc.has(p) } -> same_as<bool>;
      { cc.at(p) } -> same_as_declined<typename C::type>;
      { cc.empty() } -> same_as<bool>;
      { cc.len() } -> same_as<size_t>;
      { cc.ord() } -> ordinal;
      { c.add(t) } -> same_as<bool>;
      { c.modify(p, t) } -> same_as<bool>;
      { c.remove(p) } -> same_as<bool>;
      { c.clear() } -> same_as<void>;
    };

template <typename C>
concept char_collection = collection<C> and character<typename C::type>;

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

template <integral I>
class index_based_ordinal {
 public:
  using position = I;

 private:
  I _index;
  I _max;

 public:
  constexpr index_based_ordinal(I index, I max) : _index(index), _max(max) {}

 public:
  constexpr bool has() { return _index != _max; }
  constexpr I next() { return _index++; }
};

template <typename C>
class index_forward_input_stream {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  C& _col;
  position _pos;

 public:
  constexpr index_forward_input_stream(C& c) : _col(c), _pos(0) {}

 public:
  constexpr bool has() const { return _col.has(_pos); }
  constexpr auto next() -> decltype(auto) { return _col.at(_pos++); }
};

template <typename C>
class index_forward_output_stream {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  C& _col;

 public:
  constexpr index_forward_output_stream(C& c) : _col(c) {}

 public:
  constexpr bool add(const type& t) { return _col.add(t); }
  constexpr bool add(type&& t) { return _col.add(move(t)); }
};

template <typename C>
class index_backward_input_stream {
 public:
  using type = typename C::type;
  using position = typename C::position;

 private:
  C& _col;
  position _pos;

 public:
  constexpr index_backward_input_stream(C& c) : _col(c), _pos(_col.len() - 1) {}

 public:
  constexpr bool has() const { return _col.has(_pos); }
  constexpr auto next() -> decltype(auto) { return _col.at(_pos--); }
};

constexpr size_t prand(size_t a, size_t b, size_t seed) {
  seed = (a* seed + b) & max_of<unsigned>;
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
