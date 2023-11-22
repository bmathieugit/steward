#ifndef __steward_args_parser_hpp__
#define __steward_args_parser_hpp__

#include <core/algorithm.hpp>
#include <core/chars.hpp>
#include <core/file.hpp>
#include <core/map.hpp>
#include <core/string.hpp>

namespace args {

struct option {
  string_iterator _name;
  char _shortname;
  bool _required;
  string_iterator _fallback;
  string_iterator _help;
  string_iterator _value;
};

class program {
 private:
  string_iterator _name;
  string_iterator _location;
  map<string_iterator, option> _options;

 public:
  constexpr program(string_iterator name) : _name(name) {}

 public:
  constexpr bool parse(int argc, char** argv) {
    _location = iter(argv[0]);

    for (int i = 1; i < argc; ++i) {
    }

    return true;
  }

  constexpr void help(char_oterator auto& out) {
    write(out, "\033[1mSYNOPSIS\033[0m\n\t", _name, "\n\n");
    write(out, "\033[1mOPTIONS\033[0m\n");

    auto i = iter(_options);

    while (i.has()) {
      const auto& opt = i.next();
      write(out, "\t-", opt._shortname, ", ", opt._name, '\t', opt._help,
            "\n\n");
    }
  }

  constexpr bool add_option(string_iterator name,
                            char shortname,
                            bool required,
                            string_iterator fallback,
                            string_iterator help) {
    return _options.add(option{name, shortname, required, fallback, help},
                        name);
  }

  constexpr string_iterator get_value(string_iterator name) {
    auto i = iter(_options);

    while (i.has()) {
      const auto& opt = i.next();

      if (equals(iter(opt._name), name)) {
        return opt._fallback;
      }
    }

    return "";
  }
};

}  // namespace args

#endif