// #include <dbfile.hpp>

#include <stew.hpp>

int foo()
{
  return 24;
}

int main()
{
  using namespace stew;
  vector<int> v(1000);

  for (int i = 0; i < 1000; ++i)
  {
    v.push_back(10);
  }

  for (int i : v)
  {
    cout.printfln("{}", i);
  }

  cout.printfln("{}", all_of(v, p0 < val(0)));

  int i = 0;
  int j = 2;
  auto assign = (p0 = p1);
  stew::cout.printfln("{}", assign(i, j));
  return EXIT_SUCCESS;
}