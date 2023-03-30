#define NDEBUG

#include <stdio.h>

#include <stew/meta.hpp>
#include <stew/string.hpp>

using namespace stew;


int main(int argc, char** argv) {
  constexpr auto s0 = str::stic("aaab");
  constexpr auto s1 = str::stic("aaa");
  constexpr auto s = str::stic("");
  printf("%d\n", str::cmp(s, s1));
  return 0;
}
