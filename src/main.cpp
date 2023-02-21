#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  static_string<char, 10> s;
  for (int i : upto(0, 10))
  {
    s.push('0'+(char)i);
  } 
  stew::console<char>::printfln("\0", s);

  
  return 0;
}


