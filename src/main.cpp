// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  using namespace stew;
  int i = 0;
  int j = 2;
  auto assign = (p0 = p1);
  cout.printfln("{}", assign(i, j));
  return EXIT_SUCCESS;
}