// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  int i = 0;
  int j = 2;
  auto assign = (stew::p0 = stew::p1);
  stew::cout.printfln("{}", assign(i, j));
  return EXIT_SUCCESS;
}