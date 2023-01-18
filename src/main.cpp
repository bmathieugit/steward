#include <stew.hpp>

using namespace stew;

int main()
{
  console<char>::printfln("hello M.{}"_sv, "Mathieu"_sv);
  return 0;
}
