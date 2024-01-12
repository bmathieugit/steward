#include <args/argsparse.hpp>
#include <core/file.hpp>

int main(int argc, char** argv) {
  args::program p("steward");
  p.add_option(args::option("--help", 'h', args::option_type::flag));
  p.add_option(args::option("--version", 'v', args::option_type::flag)
                   .help("version of steward"));
  p.add_option(args::option("--database", 'd', args::option_type::string)
                   .help("file name of the database that store the properties")
                   .required()
                   .fallback("~/.steward.aresdb"));
  write(sout, p.get_flag("--help"), '\n');
  p.help(sout);
  return 0;
}
