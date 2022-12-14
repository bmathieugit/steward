// #include <dbfile.hpp>

#include <stew.hpp>

int main()
{
  stew::owning<int[]> is = new int[]{1, 2, 3};
  
  for (int i{0}; i < 3; ++i)
  {
    stew::cout.printfln("{}", is[i]);
  }

  return EXIT_SUCCESS;
}