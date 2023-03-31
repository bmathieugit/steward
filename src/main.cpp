#define NDEBUG

#include <stdio.h>

#include <stew/meta.hpp>
#include <stew/string.hpp>

using namespace stew;


int main(int argc, char** argv) {
  constexpr auto s0 = str::stic("abcdef");
  auto i = s0.riter();

  while (i.has_next()) {
    printf("%c\n", i.next());
  }

  return 0;
}
