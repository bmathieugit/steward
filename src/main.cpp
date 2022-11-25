// #include <dbfile.hpp>


#include <stew.hpp>

int main()
{
  using namespace stew;

  cout.printfln("{}", sizeofmax<int, char, size_t, string_view, int[64]>());

  return 0;
}