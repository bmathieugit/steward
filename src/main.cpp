// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{

  using namespace stew;
  
  static_assert(native_array_like<const char(&)[12]>);
  
  fstring v1("coucou");

  list<int> l;

  for (int i = 0; i < 100; ++i)
    l.push_back(i);

  for (auto &&i : l)
  {
    cout.printfln("{}  {}", i);
  }

  return EXIT_SUCCESS;
}
