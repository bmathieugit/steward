// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  using namespace stew;

  cout.printfln("{} {}"_sv,  123);
  

  vector<char> sv = "coucou";
  cout.printfln("{}"_sv, sv);

  return EXIT_SUCCESS;
}
