#ifndef __steward_args_parser_hpp__
#define __steward_args_parser_hpp__

#include <core/algorithm.hpp>
#include <core/file.hpp>
#include <core/map.hpp>
#include <core/string.hpp>

namespace args {

constexpr auto false_fallback = string_view("1");
constexpr auto true_fallback = string_view("1");

enum class option_type { string, integer, flag, none };

struct option {
  string_view _name;
  char _shortname;
  bool _required = false;
  string_view _help;
  option_type _type;
  string_view _value;

  constexpr option(string_view name, option_type type)
      : _name(name), _shortname('\0'), _type(type) {}

  constexpr option(string_view name, char shortcut, option_type type)
      : _name(name), _shortname(shortcut), _type(type) {}

  constexpr option& required() {
    _required = true;
    return *this;
  }

  constexpr option& help(string_view h) {
    _help = h;
    return *this;
  }
  constexpr option& fallback(string_view val) {
    _value = val;
    return *this;
  }
};

class program {
 private:
  string_view _name;
  string_view _location;
  map<string_view, option> _options;

 public:
  constexpr program(string_view name) : _name(name) {}

 private:
  constexpr bool int_validator(string_view val) {
    return all_of([](auto c) { return '0' <= c and c <= '9'; })(iter(val));
  }

 public:
  constexpr bool parse(int argc, char** argv) {
    _location = iter(string_view(argv[0]));

    for (int i = 1; i < argc; ++i) {
      if (_options.has(string_view(argv[i]))) {
        option& opt = _options.at(string_view(argv[i]));

        switch (opt._type) {
          case option_type::flag:
            opt._value = string_view("0");
            break;
          case option_type::integer:
            if (i < argc - 1) {
              opt._value = string_view(argv[++i]);
            }

            else {
              write(sout, "error during parsing '", opt._name, "'\n");
              return false;
            }

            break;
          case option_type::string:
            if (i < argc - 1) {
              opt._value = string_view(argv[++i]);
            }

            else {
              write(sout, "error during parsing '", opt._name, "'\n");
              return false;
            }

            break;
          case option_type::none:
            break;
        }
      }

      else {
        write(sout, "error during parsing: argument '", string_view(argv[i]),
              "' does not exist !\n");
        return false;
      }
    }

    auto iopt = iter(_options);

    while (iopt.has_next()) {
      auto&& opt = iopt.next();

      if (opt.value._required) {
        if (not opt.value._value.empty()) {
          if (opt.value._type == option_type::integer) {
            if (not int_validator(opt.value._value)) {
              write(sout, "error during parsing '", opt.value._name,
                    "': value not valid :'", opt.value._value, "'\n");
              return false;
            }
          }

          continue;
        }

        else {
          write(sout, "error during parsing '", opt.value._name,
                "': missing value\n");

          return false;
        }
      }
    }

    return true;
  }

  constexpr void help(oterator auto out) {
    write(out, "\033[1mSYNOPSIS\033[0m\n\t", _name, "\n\n");
    write(out, "\033[1mOPTIONS\033[0m\n");

    auto i = iter(_options);

    while (i.has_next()) {
      const auto& opt = i.next();
      write(out, (opt.value._required ? "\t-" : "\t[-"), opt.value._shortname,
            ", ", opt.value._name, (opt.value._required ? "\t" : "]\t"),
            opt.value._help, "\n\n");
    }
  }

  constexpr void add_option(option&& opt) {
    return _options.add(move(opt), opt._name);
  }

  constexpr void add_option(const option& opt) {
    return _options.add(opt, opt._name);
  }

  constexpr string_view get_value(string_view name) {
    if (_options.has(name)) {
      return _options.at(name)._value;
    }

    return "";
  }

  constexpr maybe<bool> get_flag(string_view name) {
    maybe<bool> b;

    if (_options.has(name)) {
      from_chars(iter(_options.at(name)._value), b);
    }

    return b;
  }

  constexpr maybe<int> get_int(string_view name) {
    maybe<int> i;

    if (_options.has(name)) {
      from_chars(iter(_options.at(name)._value), i);
    }

    return i;
  }

  constexpr maybe<string_view> get_str(string_view name) {
    maybe<string_view> i;

    if (_options.has(name)) {
      i = _options.at(name)._value;
    }

    return i;
  }
};

}  // namespace args

#endif