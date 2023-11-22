#include <args/argsparse.hpp>
#include <core/chars.hpp>
#include <core/file.hpp>

#include <core/string.hpp>

int foo(string_iterator s) {
  write(sout, s, '\n');
  return 0;
}

int main(int argc, char** argv) {
  args::program p("main program");

  p.add_option("--valid", 'v', false, "false",
               "est ce que le champ est valide ou pas");
  p.add_option("--force", 'f', true, "false",
               "est ce que le champ est forcé ou pas");

  if (argc == 1) {
    p.help(sout);
  }

  foo("coucou");

  return 0;
}
