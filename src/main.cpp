// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  using namespace stew;
  fixed_vector<int> v1(10);
  v1.push_back(1);
  v1.push_back(1);
  v1.push_back(1);
  v1.push_back(1);
  v1.push_back(1);
  cout.printfln("{}", v1.size());
  return EXIT_SUCCESS;
}