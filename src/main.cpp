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
    template <format_output O>
    static void to(O &o, const person &p)
    {
      formatter<string>::to(o, p.name);
      formatter<string>::to(o, p.fname);
    }
  };
}

int main()
{
  using namespace stew;
  string_view fmt = "Bonjour {}, tu as {}, {} chiens et {} enfants.";
  cout.printfln(fmt, "Bob", "10 ans", 2, 3);


  return 0;
}