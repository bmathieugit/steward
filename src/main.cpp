// #include <dbfile.hpp>

#include <stew.hpp>

int foo(){
  return 24;
}

int main()
{
  stew::function<int()> f = foo;
  stew::cout.printfln("{}", f());
  return EXIT_SUCCESS;
}