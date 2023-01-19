#include <stew.hpp>

using namespace stew;

int main()
{
  using cons = console<char>;
  using wcons = console<wchar_t>;

  extract_response<size_t> resp;

  // string<char> tmp;
  // maybe<char> c;

  // while ((c = termin.pop()).has() && *c != '\n')
  // {
  //   tmp.push(*c);
  // }

  cons::readf("{}"_sv, resp);
  cons::printfln("votre age est {}"_sv, *get<0>(resp));

  // if (get<0>(resp).has())
  // {
  // }


  return 0;
}
