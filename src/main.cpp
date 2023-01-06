// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

void test_transfer()
{
  int i1 = 12;
  int i2 = transfer(i1);
  cout.printfln("i1 = {} et i2 = {} ", i1, i2);
  
  int* i3 = &i2;
  int* i4 = transfer(i3);
  cout.printfln("i3 = {} et i4 = {} ", i3, i4);

  owning<int> i5 = new int{};
  *i5 = 12;
  owning<int> i6 = transfer(i5);

  cout.printfln("i5 = {} et i6 = {}({})", i5.get(), i6.get(), *i6);
}

int main()
{
  test_transfer();
  return 0;
}
