// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  stew::cout.printfln("{}", stew::p2(0, 1, 2, 3));
  return EXIT_SUCCESS;
}