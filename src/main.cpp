#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  console<char>::printfln("coucou \0 !", 12);
 
  return 0;
}
