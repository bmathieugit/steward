// #include <dbfile.hpp>

// #include <string>
// #include <vector>
// #include <iostream>
// #include <list>

#include <stew.hpp>
#include <iostream>

namespace stew
{
  std::ostream &operator<<(std::ostream &o, stew::string_view sv)
  {
    for (auto c : sv)
    {
      o << c;
    }

    return o;
  }

  std::ostream &operator<<(std::ostream &o, const stew::string& s)
  {
    for (auto c : s)
    {
      o << c;
    }

    return o;
  }
}

int main()
{
  stew::string s("prout");
  std::cout << s << "\n";
  return 0;
}