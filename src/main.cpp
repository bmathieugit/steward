// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{

  using namespace stew;

  static_assert(native_array_like<const char(&)[12]>);

  fstring v1("coucou");

  string_view s = "coucou";

  cout.printfln("{}", s);
  return EXIT_SUCCESS;
}
