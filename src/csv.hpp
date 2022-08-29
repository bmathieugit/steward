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
      std::stringstream ss;
      csv_entry<T>().from(
          t,
          [this, &ss](const auto &...i)
          { this->marshall_entry(ss, i...); });
      return ss.str();
    }

    template <typename H, typename... R>
    void marshall_entry(
        std::stringstream &ss,
        const H &h, const R &...r) const
    {
      ss << left << h << right;

      if constexpr (sizeof...(R) > 0)
      {
        ss << del;
        marshall_entry(ss, r...);
      }
    }

  public:
    template <typename T>
    T unmarshall(std::string_view s) const
    {
      T t;
      csv_entry<T>().to(t, [this, s](auto & ... i) {
        this->unmarshall_entry(s, i...);
      });
      return t;
    }

    template <typename H, typename... R>
    void unmarshall_entry(
        std::string_view s, H &h, R &...r) const
    {
      auto b =
          (s | std::views::split(del) 
             | std::views::transform([this](auto &&tk)
               {
                 return std::string_view(tk.begin() + 1, tk.end() - 1);
               })).begin();
      h = *b;
      ((r = *(++b)), ...);
    }
  };
}

#endif