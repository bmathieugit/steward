#ifndef __steward_args_parser_hpp__
#define __steward_args_parser_hpp__

#include <core/algorithm.hpp>
#include <core/file.hpp>
#include <core/map.hpp>
#include <core/string.hpp>

namespace args {

constexpr auto false_fallback = iter("1");
constexpr auto true_fallback = iter("1");

enum class option_type { string, integer, flag, none };

struct option {
  string_iterator _name;
  char _shortname;
  bool _required = false;
  string_iterator _help;
  option_type _type;
  string_iterator _value;

  constexpr option(string_iterator name, option_type type)
      : _name(name), _shortname('\0'), _type(type) {}

  constexpr option(string_iterator name, char shortcut, option_type type)
      : _name(name), _shortname(shortcut), _type(type) {}

  constexpr option& required() {
    _required = true;
    return *this;
  }

  constexpr option& help(string_iterator h) {
    _help = h;
    return *this;
  }
  constexpr option& fallback(string_iterator val) {
    _value = val;
    return *this;
  }
};

class program {
 private:
  string_iterator _name;
  string_iterator _location;
  map<string_iterator, option> _options;

 public:
  constexpr program(string_iterator name) : _name(name) {}

 private:
  constexpr bool int_validator(string_iterator val) {
    return all_of(val, [](auto c) { return '0' <= c and c <= '9'; });
  }

 public:
  constexpr bool parse(int argc, char** argv) {
    _location = iter(argv[0]);

    for (int i = 1; i < argc; ++i) {
      if (_options.has(iter(argv[i]))) {
        option& opt = _options.at(iter(argv[i]));

        switch (opt._type) {
          case option_type::flag:
            opt._value = iter("0");
            break;
          case option_type::integer:
            if (i < argc - 1) {
              opt._value = iter(argv[++i]);
            }

            else {
              write(sout, "error during parsing '", opt._name, "'\n");
              return false;
            }

            break;
          case option_type::string:
            if (i < argc - 1) {
              opt._value = iter(argv[++i]);
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
        write(sout, "error during parsing: argument '", iter(argv[i]),
              "' does not exist !\n");
        return false;
      }
    }

    auto iopt = iter(_options);

    while (iopt.has()) {
      auto&& opt = iopt.next();

      if (opt._required) {
        if (opt._value.has()) {
          if (opt._type == option_type::integer) {
            if (not int_validator(opt._value)) {
              write(sout, "error during parsing '", opt._name,
                    "': value not valid :'", opt._value, "'\n");
              return false;
            }
          }

          continue;
        }

        else {
          write(sout, "error during parsing '", opt._name,
                "': missing value\n");

          return false;
        }
      }
    }

    return true;
  }

  constexpr void help(char_oterator auto& out) {
    write(out, "\033[1mSYNOPSIS\033[0m\n\t", _name, "\n\n");
    write(out, "\033[1mOPTIONS\033[0m\n");

    auto i = iter(_options);

    while (i.has()) {
      const auto& opt = i.next();
      write(out, opt._required ? "\t-" : "\t[-", opt._shortname, ", ",
            opt._name, opt._required ? "" : "]", '\t', opt._help, "\n\n");
    }
  }

  constexpr bool add_option(option&& opt) {
    return _options.add(move(opt), opt._name);
  }

  constexpr bool add_option(const option& opt) {
    return _options.add(opt, opt._name);
  }z

  constexpr string_iterator get_value(string_iterator name) {
    if (_options.has(name)) {
      return _options.at(name)._value;
    }

    return "";
  }

  constexpr maybe<bool> get_flag(string_iterator name) {
    maybe<bool> b;

    if (_options.has(name)) {
      from_chars(_options.at(name)._value, b);
    }

    return b;
  }

  constexpr maybe<int> get_int(string_iterator name) {
    maybe<int> i;

    if (_options.has(name)) {
      from_chars(_options.at(name)._value, i);
    }

    return i;
  }

  constexpr maybe<string_iterator> get_str(string_iterator name) {
    maybe<string_iterator> i;

    if (_options.has(name)) {
      i = _options.at(name)._value;
    }

    return i;
  }
};

}  // namespace args

#endif