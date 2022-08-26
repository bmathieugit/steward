#ifndef __stew_csv_hpp__
#define __stew_csv_hpp__

#include <string>
#include <string_view>
#include <sstream>
#include <tuple>
#include <ranges>

namespace stew::csv
{
  template <typename T>
  struct csv_entry;

  struct csv_marshaller
  {
    char del = ';';
    char left = '"';
    char right = '"';

  public:
    template <typename T>
    std::string marshall(const T &t) const
    {
      return std::apply(
          [this](const auto &...attrs)
          { return this->marshall_entry(attrs...); },
          csv_entry<T>().from(t));
    }

  private:
    template <typename... I>
    std::string marshall_entry(const I &...attrs) const
    {
      std::stringstream ss;
      marshall_entry_stream(ss, attrs...);
      return ss.str();
    }

    template <typename H, typename... R>
    void marshall_entry_stream(
        std::stringstream &ss,
        const H &h, const R &...r) const
    {
      ss << left << h << right;

      if constexpr (sizeof...(R) > 0)
      {
        ss << del;
        marshall_entry_stream(ss, r...);
      }
    }

  public:
    template <typename T>
    T unmarshall(std::string_view s) const
    {
      T t;
      std::apply(
          [this, s](auto &...i)
          { this->unmarshall_entry(s, i...); },
          csv_entry<T>().to(t));
      return t;
    }

  private:
    template <typename H, typename... R>
    void unmarshall_entry(
      std::string_view s, H &h, R &...r) const
    {
      auto spl =
          s | std::views::split(del) 
            | std::views::transform([this](auto &&tk) {
                std::string_view tmp(tk.begin() + 1, tk.end() - 1);
                return tmp; 
              });

      auto b = spl.begin();
      h = *b;
      ((r = *(++b)), ...);
    }
  };

  template <typename... T>
  std::tuple<const T &...> make_csv_output(const T &...t)
  {
    return std::tuple<const T &...>{t...};
  }

  template <typename... T>
  std::tuple<T &...> make_csv_input(T &...t)
  {
    return std::tie(t...);
  }
}

#endif