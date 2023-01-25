#define NDEBUG

#include <stew.hpp>

using namespace stew;

// void foo()
// {
//   console<char>::print("foo function\n");
// }

// template <typename F>
// void benchmark(F &&f, string_view<char> desc)
// {
//   cpu_timer<time::unit::ms> cpu;

//   f();

//   console<char>::printfln("{} : {} ms", desc, cpu);
// }

struct person
{
  string<char> _name;
  size_t _age;
};



namespace stew
{
  template <>
  class formatter<xml<person>>
  {
  public:
    template <ostream O>
    constexpr static void to(O &o, const xml<person> &p)
    {
      formatter<xml_member<string<char>>>::to(o, xml_member("name", (*p)._name));

      formatter<string_view<char>>::to(o, "<person>");

      formatter<string_view<char>>::to(o, "<name>");
      formatter<string<char>>::to(o, (*p)._name);
      formatter<string_view<char>>::to(o, "</name>");

      formatter<string_view<char>>::to(o, "<age>");
      formatter<size_t>::to(o, (*p)._age);
      formatter<string_view<char>>::to(o, "</age>");

      formatter<string_view<char>>::to(o, "</person>");
    }
  };
}

int main()
{
  person p{"Jackson"_sv, 34};
  console<char>::printfln("{}", xml(p));
  console<char>::printfln("{}", xml(p));
  console<char>::printfln("{}", xml(p));
  console<char>::printfln("{}", xml(p));
  return 0;
}
