#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main(int argc, char** argv) {
  set<int> ints;
  ints.push(2);
  ints.push(4);
  ints.push(3);
  ints.push(2);

  for (int i : ints)
  {
    console<char>::printfln(str::view("\0"), i);
  }
  return 0;
}
