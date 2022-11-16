#include <dbfile.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <list>

struct address
{
  std::string _street;
  int _num;
  bool _empty;
};

struct person
{
  std::string _name;
  std::list<std::string> _firstnames;
  std::vector<address> _addresses;
};

namespace stew::dbf::storage::fs::json
{
  ostream &operator<<(ostream &sm, const address &addr)
  {
    return sm << obj(attr("street", addr._street),
                     attr("num", addr._num), 
                     attr("empty", addr._empty));
  }

  ostream &operator<<(ostream &sm, const person &p)
  {
    return sm << obj(
               attr("name", p._name),
               attr("firstnames", p._firstnames),
               attr("addresses", p._addresses));
  }
}

int main()
{
  using namespace std::literals::string_literals;

  person p;
  p._name = "martin";
  p._firstnames = {"jhon", "luc", "william"};
  p._addresses = {{"mapple street", 12, true}, {"mapple street", 221, false}};
  stew::dbf::storage::fs::json::ostream ojson;

  std::cout << (ojson << p).str();

  stew::dbf::storage::fs::json::istream ijson;

  return 0;
}