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
    constexpr static auto describe(
        string_view<char> name,
        const address &addr)
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
    constexpr static auto describe(
        string_view<char> name,
        const person &p)
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
  int i = 12;
  const int i1 = 12;
  int&& i2 = static_cast<int&&>(12);

  static_assert(same_as<decltype(relay<int&&>(12)), int&&>);
  static_assert(same_as<decltype(relay<int&>(i)), int &>);
  static_assert(same_as<decltype(relay<const int&>(i1)), const int &>);
  static_assert(same_as<decltype(relay<int&&>(static_cast<int &&>(i))), int &&>);
  static_assert(same_as<decltype(relay<int&&>(i2)), int &&>);

  return 0;
}
