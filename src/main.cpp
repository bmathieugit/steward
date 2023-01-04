// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

struct base
{
};

struct derived :public base
{
};

int main()
{
  using namespace stew;

  cout.printfln("coucou tout le monde {} ", true);

  return EXIT_SUCCESS;
}
