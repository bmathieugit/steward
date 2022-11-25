// #include <dbfile.hpp>


#include <stew.hpp>
#include <logger.hpp>

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
    template <ostream O>
    static void to(O &o, const person &p)
    {
      formatter<string>::to(o, p.name);
      formatter<string>::to(o, p.fname);
    }
  };
}

#include <sstream>

int main()
{
  using namespace stew;

  logger::debug("Bonjour {}, tu as {}, {} chiens et {} enfants.", "Bob", "10 ans", 2, 3);

  return 0;
}