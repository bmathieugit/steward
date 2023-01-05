// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  using namespace stew;

  stack<int> is(100);

  for (int i = 0; i < 1000; i++)
  {
    is.push(i);
  }

  for (int i : is)
  {
    printf("%d\n", i);
  }

  return EXIT_SUCCESS;
}
