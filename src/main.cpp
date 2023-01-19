#include <stew.hpp>

using namespace stew;

int main()
{
  using cons = console<char>;

  cons::printfln("time = {}"_sv,   wall_timer());
  cons::printfln("time = {}"_sv,   cpu_timer<time::unit::ns>());

  return 0;
}
