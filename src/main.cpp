// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

void test_transfer()
{
  int i1 = 12;
  int i2 = transfer(i1);
  cout.printfln("i1 = {} et i2 = {} ", i1, i2);

  int *i3 = &i2;
  int *i4 = transfer(i3);
  cout.printfln("i3 = {} et i4 = {} ", i3, i4);

  owning<int> i5 = new int{12};
  owning<int> i6 = transfer(i5);

  cout.printfln("i5 = {} et i6 = {}({})", i5.get(), i6.get(), *i6);
}

void test_fixed_vector()
{
  fixed_vector<int> ints1(upto(0, 10));

  for (int i : ints1)
  {
    cout.printf("{}", i);
  }

  cout.printf("\n");

  fixed_vector<int> ints2 = transfer(ints1);

  cout.printfln("size ints1 {}, size int2 {}", ints1.size(), ints2.size());

  for (int i : ints1)
  {
    cout.printf("{}", i);
  }
}

void test_copy_algorithm()
{
  fixed_vector<int> ints(upto(0, 10));
  vector<int> ints2(10);

  copy(ints, push_back_inserter(ints2));

  for (int i : ints)
  {
    cout.printf("{}", i);
  }

  cout.printf("\n");

  for (int i : ints2)
  {
    cout.printf("{}", i);
  }

  cout.printf("\n");

  fixed_stack<int> ints3(upto(0, 10));
  fixed_stack<int> ints4;
  copy(ints3, push_inserter(ints4));

  for (int i : ints3)
  {
    cout.printf("{}", i);
  }

  cout.printf("\n");

  for (int i : ints4)
  {
    cout.printf("{}", i);
  }

  cout.printf("\n");
}

int main()
{
  test_transfer();
  test_fixed_vector();
  test_copy_algorithm();

  return 0;
}
