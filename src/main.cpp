// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  using namespace stew;



  int i = 0;
  int j = 2;
  auto assign = (p0 = p1);
  stew::cout.printfln("{}", assign(i, j));
  return EXIT_SUCCESS;
}