// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  stew::async(stew::asynced, [](auto i){stew::cout.printfln("hello {}", i); }, "Bob").get();

  return EXIT_SUCCESS;
}