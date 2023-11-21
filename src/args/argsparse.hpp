#ifndef __steward_args_parser_hpp__
#define __steward_args_parser_hpp__

#include <core/algorithm.hpp>
#include <core/chars.hpp>
#include <core/file.hpp>
#include <core/map.hpp>
#include <core/string.hpp>

namespace args {

struct option {
  string _name;
  char _shortname;
  bool _required;
  string _fallback;
  string _help;
  string _value;
};

class program {
 private:
  string _name;
  string _location;
  map<string, option> _options;

 public:
  constexpr program(char_iterator auto name) : _name(name) {}

 public:
  constexpr bool parse(int argc, char** argv) {
    _location = iter(argv[0]);

    for (int i = 1; i < argc; ++i) {
    }

    return true;
  }

  constexpr string help() {
    auto buff = string();
    auto out = oter(buff);

    write(out, "\033[1mSYNOPSIS\033[0m\n\t", _name, "\n\n");
    write(out, "\033[1mOPTIONS\033[0m\n");

    auto i = iter(_options);

    while (i.has()) {
      const auto& opt = i.next();
      write(out, "\t-", opt._shortname, ", ", opt._name, '\t', opt._help,
            "\n\n");
    }

    return buff;
  }

  constexpr bool add_option(char_iterator auto name,
                            char shortname,
                            bool required,
                            char_iterator auto fallback,
                            char_iterator auto help) {
    return _options.add(option{name, shortname, required, fallback, help},
                        name);
  }

  constexpr string get_value(char_iterator auto name) {
    auto i = iter(_options);

    while (i.has()) {
      const auto& opt = i.next();

      if (equals(iter(opt._name), name)) {
        return opt._fallback;
      }
    }

    return iter("");
  }
};

}  // namespace args

#endif