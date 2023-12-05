#include <stdio.h>
#include <core/core.hpp>
#include <core/vector.hpp>

int main(int argc, char** argv) {
  vector<int> v;
  v.add(1);
  v.add(2);
  v.add(3);
  auto b = begin(v);
  auto e = end(v);

  while (b != e) {
    printf("%i\n", *(b++));
  }

  return 0;
}
