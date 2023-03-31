#ifndef __steward_format_hpp__
#define __steward_format_hpp__

#include <stew/meta.hpp>
#include <stew/meta/container.hpp>
#include <stew/meta/iterable.hpp>
#include <stew/meta/iterator.hpp>
#include <stew/string.hpp>
#include <stew/view.hpp>

namespace stew {

template <typename O>
concept ostream = push_container<O, char> || push_container<O, wchar_t>;

template <typename T>
class formatter;

template <typename T, typename O>
concept formattable =
    ostream<O> && requires(const T &t, O &o) { formatter<T>::to(o, t); };

template <ostream O, formattable<O> T>
constexpr void format_one_to(O &o, const T &t) {
  formatter<rm_cvref<T>>::to(o, t);
}

template <character C>
constinit const C joker = '$';

namespace impl {
template <ostream O, typename H>
constexpr void format_to_one_by_one(O &o, iterator auto &ifmt, const H &h) {
  while (ifmt.has_next()) {
    auto c = ifmt.next();

    if (c == '$') {
      break;
    } else {
      format_one_to(o, c);
    }
  }

  format_one_to(o, h);
}

template <character C, ostream O, typename... T>
constexpr void format_to(O &o, string_view<C> fmt, const T &...t) {
  auto ifmt = fmt.iter();
  (format_to_one_by_one(o, ifmt, t), ...);
  while (ifmt.has_next()) format_one_to(o, ifmt.next());
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

template <signed_integral I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O &o, I i) {
    static_vector<char, 20> tbuff;

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

    format_one_to(o, view(tbuff.riter()));
  }
};

template <unsigned_integral I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O &o, I i) {
    static_vector<char, 20> tbuff;

    if (i == 0) {
      tbuff.push('0');
    } else {
      while (i != 0) {
        tbuff.push("0123456789"[i % 10]);
        i /= 10;
      }
    }

    format_one_to(o, view(tbuff.riter()));
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
    constexpr auto _true = str::view("true");
    constexpr auto _false = str::view("false");
    format_one_to(o, b ? _true : _false);
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

template <iterable I>
class formatter<I> {
 public:
  template <ostream O>
  constexpr static void to(O &o, const I &i) {
    iterator auto ii = i.iter();
    while (ii.has_next()) {
      o.push(ii.next());
    }
  }
};
}  // namespace stew

#endif
