#include <args/argsparse.hpp>
#include <core/file.hpp>
#include <core/iterator.hpp>
#include <core/string.hpp>

int main(int argc, char** argv) {
  args::program p("main program");

  p.add_option(args::option("--valid", 'v', args::option_type::flag)
                   .help("est ce que le champ est valide ou pas")
                   .fallback(args::false_fallback));
  p.add_option(args::option("--force", 'f', args::option_type::flag)
                   .fallback(args::false_fallback)
                   .help("est ce que le champ est forcé ou pas"));
  p.add_option(args::option("--file", 'i', args::option_type::string)
                   .required()
                   .help("est ce que le champ est forcé ou pas"));

  if (not p.parse(argc, argv)) {
    p.help(sout);
  }

  write(sout, p.get_flag("--valid").get(), '\n');
  
  return 0;
}
