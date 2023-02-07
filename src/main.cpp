#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  set<int> s;
  s.push(9);
  s.push(12);
  s.push(125);
  s.push(10);
  s.push(4);

  list<int>::node *b = s._first;

  for (int i : s)
  {
    console<char>::printfln("{}", i);
  }

  return 0;
}
