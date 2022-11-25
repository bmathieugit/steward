#ifndef __stew_logger_hpp__
#define __stew_logger_hpp__

#include <array>
#include <stew.hpp>

namespace stew::logger
{
  enum class level : int
  {
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    fatal = 5,
    stew = 6
  };
}

namespace stew
{
  template <>
  class formatter<logger::level>
  {
  public:
    template <ostream O>
    static void to(O &o, logger::level l)
    {
      string_view ltable[7] = {
          "trace", "debug", "info",
          "warn", "error", "fatal",
          "steward"};

      formatter<string_view>::to(o, ltable[(int)l]);
    }
  };
}

namespace stew::logger
{

  template <ostream O, typename... T>
  void flog(O &o, level l, string_view &mess, const T &...t)
  {
    format_to(o, "[{}] : ", l);

    if constexpr (sizeof...(T) > 0)
    {
      format_to(o, mess, t...);
    }
    else
    {
      o.push_all(mess);
    }

    o.push_one('\n');
  }

  template <ostream O>
  void ftrace(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::trace, mess, args...);
  }

  template <ostream O>
  void fdebug(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::debug, mess, args...);
  }

  template <ostream O>
  void finfo(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::info, mess, args...);
  }

  template <ostream O>
  void fwarn(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::warn, mess, args...);
  }

  template <ostream O>
  void ferror(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::error, mess, args...);
  }

  template <ostream O>
  void ffatal(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::fatal, mess, args...);
  }

  template <ostream O>
  void fstew(O &o, string_view mess, const auto &...args)
  {
    flog(o, level::stew, mess, args...);
  }

  void trace(string_view mess, const auto &...args)
  {
    ftrace(cout, mess, args...);
  }

  void debug(string_view mess, const auto &...args)
  {
    fdebug(cout, mess, args...);
  }

  void info(string_view mess, const auto &...args)
  {
    finfo(cout, mess, args...);
  }

  void warn(string_view mess, const auto &...args)
  {
    fwarn(cout, mess, args...);
  }

  void error(string_view mess, const auto &...args)
  {
    ferror(cout, mess, args...);
  }

  void fatal(string_view mess, const auto &...args)
  {
    ffatal(cout, mess, args...);
  }

  void stew(string_view mess, const auto &...args)
  {
    fstew(cout, mess, args...);
  }
}

#endif