#include <args/argsparse.hpp>
#include <core/file.hpp>
#include <core/string.hpp>

int main(int argc, char** argv) {
  args::program p("main program");

  p.add_option("--valid", 'v', args::option_type::flag, false, "false",
               "est ce que le champ est valide ou pas");
  p.add_option("--force", 'f', args::option_type::flag, false, "false",
               "est ce que le champ est forcé ou pas");
  p.add_option("--file", 'i', args::option_type::string, true, "",
               "nom d'un fichier quelconque");

  if (not p.parse(argc, argv)) {
    p.help(sout);
  }
  
  write(sout, p.get_value("--file"), '\n');
  return 0;
}
