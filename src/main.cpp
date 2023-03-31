#define NDEBUG

#include <stdio.h>
#include <stew/string.hpp>
#include <stew/format.hpp>

using namespace stew;


int main(int argc, char** argv) {
  constexpr auto s0 = str::stic("abcdef");
  
  string<char> s1(100);

  format_to(s1, str::view("my string $ hhii $ !"), s0, s0);

  auto i = s1.iter();

  while (i.has_next()) {
    printf("%c", i.next());
  }

  return 0;
}
