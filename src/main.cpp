// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  constexpr auto add = stew::p1 + stew::p2 + stew::p2 - stew::p0;
  stew::cout.printfln("{}", add(1, 2, 3));
  return EXIT_SUCCESS;
}