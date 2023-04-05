#define NDEBUG

#include <stew/iofile.hpp>

using namespace stew;

int main(int argc, char** argv) {
  constexpr auto s0 = str::stic("abcdef");
  
  io::printfln(str::view("ma string est comme suit : '$'"), s0);

  return 0;
}
