#ifndef __steward_format_hpp__
#define __steward_format_hpp__

#include <stew/algorithm.hpp>
#include <stew/iterator.hpp>
#include <stew/stack.hpp>
#include <stew/string.hpp>

namespace stew {

template  <typename O>
concept ostream = push_container<O, char> || push_container<O, wchar_t>;

template <typename T>
class formatter;

template <typename T, typename O>
concept formattable =
    ostream<O> && requires(const T &t, O &o) { formatter<T>::to(o, t); };

template <ostream O, formattable<O> T>
constexpr void format_one_to(O &o, const T &t) {
  formatter<T>::to(o, t);
}

template <character C>
constinit const C joker = '$';

namespace impl {
template <character C, ostream O, typename H>
constexpr void format_to_one_by_one(O &o, string_view<C> &fmt, const H &h) {
  auto b = fmt.begin();
  auto e = fmt.end();

  while (b != e) {
    if (*b == joker<C>) {
      ++b;
      break;
    } else {
      format_one_to(o, *b);
      ++b;
    }
  }

  format_one_to(o, h);
  fmt = string_view<C>(b, e);
}

template <character C, ostream O, typename... T>
constexpr void format_to(O &o, string_view<C> fmt, const T &...t) {
  (format_to_one_by_one(o, fmt, t), ...);
  format_one_to(o, fmt);
}
}  // namespace impl

template <ostream O, typename... A>
constexpr void format_to(O &o, string_view<char> fmt, const A &...a) {
  impl::format_to(o, fmt, a...);
}

template <ostream O, typename... A>
constexpr void format_to(O &o, string_view<wchar_t> fmt, const A &...a) {
  impl::format_to(o, fmt, a...);
}

template <character C>
class formatter<C> {
 public:
  template <ostream O>
  constexpr static void to(O &os, C o) {
    os.push(o);
  }
};

template <typename I>
class formatter<view<I>> {
 public:
  template <ostream O>
  constexpr static void to(O &o, const view<I> &v) {
    for (const auto &i : v) {
      format_one_to(o, i);
    }
  }
};

template <character C, size_t N>
class formatter<const C (&)[N]> : public formatter<string_view<C>> {};

template <character C, size_t N>
class formatter<C (&)[N]> : public formatter<string_view<C>> {};

template <character C, size_t N>
class formatter<const C[N]> : public formatter<string_view<C>> {};

template <character C, size_t N>
class formatter<C[N]> : public formatter<string_view<C>> {};

template <character C, size_t N>
class formatter<static_string<C, N>> : public formatter<string_view<C>> {};

template <character C>
class formatter<fixed_string<C>> : public formatter<string_view<C>> {};

template <character C>
class formatter<string<C>> : public formatter<string_view<C>> {};

template <signed_integral I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O &o, I i) {
    static_stack<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      const bool neg = i < 0;
      I tmp = neg ? -i : i;

      while (tmp != 0) {
        tbuff.push("0123456789"[tmp % 10]);
        tmp /= 10;
      }

      if (neg) {
        tbuff.push('-');
      }
    }

    format_one_to(o, view(tbuff));
  }
};

template <unsigned_integral I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O &o, I i) {
    static_stack<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      while (i != 0) {
        tbuff.push("0123456789"[i % 10]);
        i /= 10;
      }
    }

    format_one_to(o, view(tbuff));
  }
};

template <floating_point F>
class formatter<F> {
 public:
  template <ostream O>
  constexpr static void to(O &o, F d) {
    size_t i = static_cast<size_t>(d);
    size_t e = static_cast<size_t>((d - i) * 10'000.0);

    format_one_to(o, i);
    format_one_to(o, '.');
    format_one_to(o, e);
  }
};

template <>
class formatter<bool> {
 public:
  template <character C, ostream O>
  constexpr static void to(O &o, bool b) {
    format_one_to(o, b ? str::view("true") : str::view("false"));
  }
};

template <pointer_eq P>
class formatter<P> {
 public:
  template <ostream O>
  constexpr static void to(O &os, P p) {
    format_one_to(os, (size_t)(void *)(p));
  }
};

}  // namespace stew

#endif
