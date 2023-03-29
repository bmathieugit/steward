#define NDEBUG

#include <stdio.h>

#include <stew/vector.hpp>

using namespace stew;

void for_each(auto&& iterable, auto&& func) {
  auto i = iterable.iter();
  while (i.has_next()) {
    func(i.next());
  }
}

int main(int argc, char** argv) {
  stew::ext_vector<int> v(10);

  v.push(0);
  v.push(1);

  for_each(v, [](const int& i) { printf("%d\n", i); });

  return 0;
}
