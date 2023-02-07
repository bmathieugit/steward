#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  list<int> is;

  is.push(array<int, 3>{0, 1, 2});
  is.pop();
  is.push(12);

  for (int &i : is)
  {
    console<char>::printfln("{}", i);
  }

  local<static_string<char, 10>> s = new static_string<char, 10>();
  *s = "coucou"_sv;

  console<char>::printfln("{}", *s.get());

  return 0;
}
