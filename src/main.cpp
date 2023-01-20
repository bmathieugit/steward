#include <stew.hpp>

using namespace stew;

int main()
{
  console<char>::printfln("heure locale : {}"_sv, date<time::zone::local>());
  return 0;
}
