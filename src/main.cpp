// #include <dbfile.hpp>

#include <stew.hpp>

int main()
{
  stew::vector<int> v(4);
  
  for (int i = 0; i < 4; ++i)
    v.push_back(i);
  
  for (const int&  i : v)
    stew::cout.printfln("{}", v[i]);

  return EXIT_SUCCESS;
}