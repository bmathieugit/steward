#define NDEBUG

#include <stdio.h>

#include <stew/meta.hpp>
#include <stew/string.hpp>

using namespace stew;

void for_each(auto&& iterable, auto&& func) {
  auto i = iterable.iter();
  while (i.has_next()) {
    func(i.next());
  }
}


int main(int argc, char** argv) {
  auto v = str::view("coucou");
 auto s = str::str("coucou");
  for_each(v, [](const int& i) { printf("%c\n", i); });

  return 0;
}
