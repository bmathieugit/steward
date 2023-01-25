#include <stew.hpp>

using namespace stew;

int main()
{
  console<char>::print("quel est votre age ?");
  extract_response<int> res;
  console<char>::readf("{}", res);
  console<char>::printfln("votre reponse est {}", res.get<0>().operator*());

  return 0;
}
