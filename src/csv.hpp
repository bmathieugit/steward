#ifndef __stew_csv_hpp__
#define __stew_csv_hpp__

#include <string>
#include <string_view>

#include <sstream>
#include <fstream>

#include <iostream>

namespace stew::csv
{

  template <typename T>
  struct csv_entry;

  struct csv_marshaller
  {
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
      ss << '"' << h << '"';

      if constexpr (sizeof...(R) > 0)
      {
        ss << ';';
        marshall_entry(ss, r...);
      }
    }

  public:
    template <typename T>
    T unmarshall(std::string_view s) const
    {
      T t;
      csv_entry<T>().to(t, [this, s](auto &...i)
                        { this->unmarshall_entry(s, i...); });
      return t;
    }

    template <typename H, typename... R>
    void unmarshall_entry(
        std::string_view line, H &h, R &...r) const
    {
      std::size_t i = line.find(';');
      std::string_view item = i == std::string_view::npos ? line : line.substr(0, i);

      if (item.size() > 2)
      {
        item.remove_prefix(1);
        item.remove_suffix(1);

        h = item;

        if constexpr (sizeof...(R))
        {
          line.remove_prefix(
              std::min(item.size() + 3,
                       line.size()));
          unmarshall_entry(line, r...);
        }
      }
    }
  };
}

#endif