#include <stdio.h>
#include <core/core.hpp>
#include <core/vector.hpp>

int main(int argc, char** argv) {
  vector<int> v;
  v.add(1);
  v.add(2);
  v.add(3);
  auto it = iter(v);
  while (it.has_next()) {
    printf("%i\n", it.next());
  }

  return 0;
}
