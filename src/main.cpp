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
  template <typename T>
  stream &operator<<(stream &sm, const std::list<T> &l)
  {
    std::size_t i = 0;
    auto b = l.begin();
    auto e = l.end();

    sm << '[';

    while (b != e)
    {
      sm << *b;

      if (i < l.size() - 1)
      {
        sm << ',';
      }

      ++i;
      ++b;
    }

    return sm << ']';
  }

  stream &operator<<(stream &sm, const address &addr)
  {
    return sm << obj(attr("street", addr._street),
                     attr("num", addr._num), 
                     attr("empty", addr._empty));
  }

  stream &operator<<(stream &sm, const person &p)
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
  stew::dbf::storage::fs::json::stream json;

  std::cout << (json << p).str();

  return 0;
}