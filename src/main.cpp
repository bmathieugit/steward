// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  file<char, io_open::r> makefile("makefile"_sv);

  termout.push(makefile);

  return 0;
}
