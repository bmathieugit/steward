#include <stew.hpp>

using namespace stew;

int main()
{
  using cons = console<char>;

  date<time::zone::local> now;

  cons::printfln("heure locale : {}"_sv, now);
  return 0;
}
