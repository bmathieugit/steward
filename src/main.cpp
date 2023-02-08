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
  
  s.remove(find(s, 4));

  for (int i : s)
  {
    console<char>::printfln("{}", i);
  }



  return 0;
}
