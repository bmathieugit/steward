#include <args/argsparse.hpp>
#include <core/file.hpp>
#include <ui/ui.hpp>

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

  ui::screen scr(ui::dimensions{10, 10});

  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 10; ++x) {
      scr.at(x, y)._content = 'a';
    }
  }

  ui::render(sout, scr);

  return 0;
}
