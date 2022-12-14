// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  stew::cout.printfln("{}", stew::defer(foo).get());
  stew::cout.printfln("{}", stew::async(foo).get());

  return EXIT_SUCCESS;
}