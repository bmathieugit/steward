#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  bdd::list<int> is;
  is.push(12);
  is.push(14);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);
  is.push(15);

  is.insert(0, is.begin());

  for (int &i : is)
  {
    console<char>::printfln("{}", i);
  }
  return 0;
}
