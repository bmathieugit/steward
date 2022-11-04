#include <dbfile.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <list>


struct person
{
  std::string _name;
  std::list<std::string> _firstnames;
};

namespace stew::dbf::storage::fs::json
{
  template <typename T>
  std::string to_json(const std::list<T> &l)
  {
    std::string s;
    s.push_back('[');

    for (const auto &i : l)
    {
      s.append(to_json(i));
      s.push_back(',');
    }

    s.pop_back();
    s.push_back(']');

    return s;
  }

  std::string to_json(const person &p)
  {
    return to_json(
        attr("name", p._name),
        attr("firstnames", p._firstnames));
  }
}

int main()
{
  using namespace std::literals::string_literals;

  person p;
  p._name = "martin";
  p._firstnames = {"jhon", "luc", "william"};

  std::cout << stew::dbf::storage::fs::json::to_json(p);

  return 0;
}