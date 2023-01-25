#include <stew.hpp>

using namespace stew;

int main()
{
  console<char>::print("quel est votre age ?");
  console<char>::printfln("votre reponse est {}", buff);

  return 0;
}
