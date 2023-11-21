#include <args/argsparse.hpp>
#include <core/chars.hpp>
#include <core/file.hpp>

#include <core/string.hpp>

int foo(string_view auto& s) {
  write(sout, iter(s), '\n');
  return 0;
}

int main(int argc, char** argv) {
  args::program p(iter("main program"));

  p.add_option(iter("--valid"), 'v', false, iter("false"),
               iter("est ce que le champ est valide ou pas"));
  p.add_option(iter("--force"), 'f', true, iter("false"),
               iter("est ce que le champ est forcé ou pas"));

  if (argc == 1) {
    write(sout, p.help());
  }

  foo("coucou");

  return 0;
}
