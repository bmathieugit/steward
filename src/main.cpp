#include <stew.hpp>

using namespace stew;

int main()
{
  using cons = console<char>;


  double d = 1234555.399203;
  cons::printfln("time = {}"_sv, d);

  return 0;
}
