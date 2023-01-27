#define NDEBUG

#include <stew.hpp>

using namespace stew;

struct address
{
  string_view<char> _street;
};

namespace stew
{
  template <>
  class xml_descriptor<address>
  {
  public:
    constexpr static auto describe(string_view<char> name, const address &addr)
    {
      return make_xml_node(
          name,
          xml_describe("street", addr._street));
    }
  };
}

struct person
{
  string_view<char> _name;
  string_view<char> _firstname;
  address _addr;
};

namespace stew
{
  template <>
  class xml_descriptor<person>
  {
  public:
    constexpr static auto describe(string_view<char> name, const person &p)
    {
      return make_xml_node(
          name,
          xml_describe("name"_sv, p._name),
          xml_describe("firstname"_sv, p._firstname),
          xml_describe("address", p._addr));
    }
  };
}

int main()
{
  person p{"MARLEY"_sv, "Bob"_sv, {"mapple street"_sv}};
  console<char>::printfln("{}", pretty<0, 2>(xml_describe("person", p)));

  return 0;
}
