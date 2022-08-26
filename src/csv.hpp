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

 
 /*  template<typename T, >
  concept csv_convertible = 
    requires(const T& t, T& t2, std::stringstream& ss) 
  {
    csv_entry<T>().from(t, ss, )
  }; */
  
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
      csv_entry<T>().from(t, ss, *this);
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
      csv_entry<T>().to(t, s, *this);
      return t;
    }

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
}

#endif