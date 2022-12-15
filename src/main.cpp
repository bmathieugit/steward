// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  using namespace stew;

  function<int()> f = foo;
  cout.printfln("{}", f());

  // int i = 0;
  // int j = 2;
  // auto assign = (p0 = p1);
  // stew::cout.printfln("{}", assign(i, j));
  return EXIT_SUCCESS;
}