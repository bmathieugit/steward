#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  bdd::list<int> is;

  is.push(array<int, 3>{0, 1, 2});
  is.pop();
  is.push(12);

  for (int &i : is)
  {
    console<char>::printfln("{}", i);
  }

  return 0;
}
