#ifndef __stew_csv_hpp__
#define __stew_csv_hpp__

#include <string>
#include <string_view>
#include <sstream>
#include <tuple>
#include <ranges>

namespace stew::csv
{
  namespace impl
  {
    template <typename H, typename... T>
    void to_csv(
        std::ostringstream &ss,
        char del,
        char l,
        char r,
        const H &h,
        const T &...t)
    {
      ss << l << h << r;

      if constexpr (sizeof...(t) > 0)
      {
        ss << del;
        stew::csv::impl::to_csv(ss, del, l, r, t...);
      };
    }
  }

  template <typename T>
  std::string to_csv(
      const T &t,
      char del = ';',
      char l = '"',
      char r = '"')
  {
    std::ostringstream ss;
    std::apply(
        [del, l, r, &ss](auto &&...i)
        { impl::to_csv(ss, del, l, r, i...); },
        to_csv_output(t));
    return std::move(ss).str();
  }

  template <typename... T>
  std::tuple<const T &...> make_csv_output(const T &...t)
  {
    return std::tuple<const T &...>{t...};
  }

  namespace impl
  {
    template <typename H, typename... T>
    void from_csv(
        std::string_view line,
        char del,
        char l,
        char r,
        H &h,
        T &...t)
    {
      auto split =
          line | std::views::split(del) | std::views::transform([](auto &&tk)
                                                                {
                  std::string_view tmp(tk.begin(), tk.end());
                  tmp.remove_prefix(1);
                  tmp.remove_suffix(1);
                  return tmp; });
      auto b = split.begin();
      h = *b;
      ((t = *(++b)), ...);
    }
  }

  template <typename T>
  T from_csv(
      std::string_view line,
      char del = ';',
      char l = '"',
      char r = '"')
  {
    T t;
    std::apply(
        [del, l, r, &line](auto &&...t)
        { impl::from_csv(line, del, l, r, t...); },
        to_csv_input(t));
    return t;
  }

  template <typename... T>
  std::tuple<T &...> make_csv_input( T &...t)
  {
    return std::tie(t...);
  }
}

#endif