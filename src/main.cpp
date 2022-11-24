// #include <dbfile.hpp>

// #include <string>
// #include <vector>
// #include <iostream>
// #include <list>

#include <stew.hpp>
//#include <iostream>

struct person
{
  stew::string name;
  stew::string fname;
};

namespace stew
{
  template <>
  class formatter<person>
  {
  public:
    static void to(string &os, const person &p)
    {
      formatter<string>::to(os, p.name);
      formatter<string>::to(os, p.fname);
    }
  };
}

namespace stew
{
  

}

int main()
{
  using namespace stew;

  string s("prout");
  string_view sv("prout");

  person bob{"Dilan", "Bob"};

  cout.write(format(" hello {} tu es le {} eme {}!\n", bob, 2u, true));

  return 0;
}