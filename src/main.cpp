// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  stew::cout.printfln("{}", stew::async(stew::asynced, [](auto i){stew::cout.printfln("hello {}", i); return i;}, "Bob").get());

  return EXIT_SUCCESS;
}