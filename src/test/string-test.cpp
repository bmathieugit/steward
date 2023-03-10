#include <stew.hpp>

using namespace stew;

int main() {
  console<char>::printfln(str::view("view size of 'coucou' : \0"), str::view("coucou").size());
  console<char>::printfln(str::view("fixed_string size of 'coucou' : \0"), str::fixed("coucou").size());
  console<char>::printfln(str::view("string size of 'coucou' : \0"), str::str("coucou").size());
}
