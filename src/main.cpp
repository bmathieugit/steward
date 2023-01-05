// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  using namespace stew;

  stack<int> is(100);

  for (int i : sequence(0, 1000, (p0 + val(1))))
  {
    is.push(i);
  }

  for (int i : is)
  {
    printf("%d\n", i);
  }

  return EXIT_SUCCESS;
}
