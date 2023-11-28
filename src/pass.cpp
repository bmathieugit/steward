#include <args/argsparse.hpp>
#include <core/file.hpp>
#include <core/string.hpp>
#include <vault/crypto/ares.hpp>
#include <vault/crypto/mash.hpp>

int main(int argc, char** argv) {
  args::program p("pass tool");

  p.add_option(args::option("--master", 'm', args::option_type::string)
                   .required()
                   .help("master password to encrypt/decrypt database"));
  p.add_option(args::option("--database", 'd', args::option_type::string)
                   .required()
                   .fallback("~/.passdb")
                   .help("database file to read/write"));
  p.add_option(args::option("--key", 'k', args::option_type::string)
                   .required()
                   .help("key to consult or update or append into database"));
  p.add_option(args::option("--value", 'v', args::option_type::string)
                   .help("value to update or to append"));

  if (not p.parse(argc, argv)) {
    write(serr, "error during parsing args line\n");
    p.help(sout);
    return 1;
  }

  auto dbpath = p.get_str("--database").get();
  file<uchar, mode::r> f(dbpath);

  if (not f.opened()) {
    write(serr, "error during opening file ", dbpath);
    return 1;
  }

  fixed_string dbdata(f.len());
  f.readall(oter(dbdata));

  write(sout, dbdata);
}