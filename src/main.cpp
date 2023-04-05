#define NDEBUG

#include <stew/iofile.hpp>

using namespace stew;

int main(int argc, char** argv) {
  string<char> s(10);
  string<char> ss(10);

  push(s, 'a', 'b', 'c', 'd', 'f', 'g');
  copy(s, ss);
  io::printfln(str::view("ma string est comme suit : '$'"), s);
  io::printfln(str::view("ma sstring est comme suit : '$'"), ss);

  return 0;
}
