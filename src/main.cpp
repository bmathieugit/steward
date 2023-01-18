#include <stew.hpp>

using namespace stew;

int main()
{
  string<char> coucou("coucou"_sv);
  termout.push(coucou);

  using cons = console<char>;
  using wcons = console<wchar_t>;
  
  extract_response<size_t> resp;
  
  cons::readf("{}"_sv, resp);
  cons::printfln("votre age est {}"_sv, *get<0>(resp));

  if (get<0>(resp).has())
  {
  }

  return 0;
}
