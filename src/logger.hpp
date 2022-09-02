#ifndef __stew_logger_hpp__
#define __stew_logger_hpp__

#include <array>
#include <iostream>
#include <string_view>

namespace stew::logger
{
  void flog(std::ostream& o, const auto&... args)
  {
    ((o << ... << args) << '\n');
  }

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

  template <std::size_t n>
  using level_table =
      std::array<std::string_view, n>;

  void flog(std::ostream &o, level l, const auto &...args)
  {
    constexpr level_table<8> ltable = {
        "trace", "debug", "info",
        "warn", "error", "fatal",
        "steward"};
    o << ltable[static_cast<int>(l)] << " : ";
    (o << ... << args);
    o << '\n';
  }

  void ftrace(std::ostream &o, const auto &...args)
  {
    flog(o, level::trace, args...);
  }

  void fdebug(std::ostream &o,
              const auto &...args)
  {
    flog(o, level::debug, args...);
  }

  void finfo(std::ostream &o,
             const auto &...args)
  {
    flog(o, level::info, args...);
  }

  void fwarn(std::ostream &o,
             const auto &...args)
  {
    flog(o, level::warn, args...);
  }

  void ferror(std::ostream &o,
              const auto &...args)
  {
    flog(o, level::error, args...);
  }

  void ffatal(std::ostream &o,
              const auto &...args)
  {
    flog(o, level::fatal, args...);
  }

  void fstew(std::ostream &o,
             const auto &...args)
  {
    flog(o, level::stew, args...);
  }

  void trace(const auto &...args)
  {
    ftrace(std::cout, args...);
  }

  void debug(const auto &...args)
  {
    fdebug(std::cout, args...);
  }

  void info(const auto &...args)
  {
    finfo(std::cout, args...);
  }

  void warn(const auto &...args)
  {
    fwarn(std::cout, args...);
  }

  void error(const auto &...args)
  {
    ferror(std::cout, args...);
  }

  void fatal(const auto &...args)
  {
    ffatal(std::cout, args...);
  }

  void stew(const auto &...args)
  {
    fstew(std::cout, args...);
  }

}

#endif