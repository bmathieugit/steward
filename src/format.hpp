#ifndef __stew_format_hpp__
#define __stew_format_hpp__

#include <string>
#include <string_view>
#include <sstream>
#include <concepts>

namespace std
{
  template <typename H, typename... T>
  void format_impl(std::string_view tail, std::stringstream &ss, H &&h, T &&...t)
  {
    std::string::size_type pos = tail.find("{}");

    if (pos != std::string::npos)
    {
      ss << tail.substr(0, pos);
      ss << std::forward<H>(h);

      if constexpr (sizeof...(T) != 0)
      {
        format_impl(tail.substr(pos + 2), ss, std::forward<T>(t)...);
      }
      else
      {
        ss << tail.substr(pos + 2);
      }
    }
    else
    {
      ss << tail;
    }
  }

  template <typename... T>
  std::string format(std::string_view pattern, T &&...t)
  {
    std::stringstream ss;
    format_impl(pattern, ss, std::forward<T>(t)...);
    return ss.str();
  }
}

#endif