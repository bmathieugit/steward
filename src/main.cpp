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
  fixed_vector<int> ints2(10);

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
  copy(transfer(ints3), push_inserter(ints4));

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

struct boo
{
  int i;
  ~boo() = default;
  boo()
  {
  }

  boo(int _i) : i(_i)
  {
  }

  boo(const boo &o)
  {
    i = o.i;
  }

  boo(boo &&o)
  {
    i = transfer(o.i);
  }

  boo &operator=(const boo &o)
  {
    i = o.i;
    return *this;
  }

  boo &operator=(boo &&o)
  {
    i = transfer(o.i);
    return *this;
  }
};

void test_move_view()
{
  fixed_vector<boo> ss(10);
  ss.push_back(boo(1));
  ss.push_back(boo(2));

  cout.printfln("ss size {}", ss.size());

  decltype(ss) ss2(ss.size());

  copy(transfer_view(ss.begin(), ss.end()), push_back_inserter(ss2));

  cout.printfln("ss size {}", ss.size());
  cout.printfln("ss2 size {}", ss2.size());
}

void test_reverse_iterator()
{
  fixed_vector<int> is(upto(0, 10));

  cout.printfln("uptosize {}", is.size());

  for (int i : is)
  {
    cout.printfln(" is {}", i);
  }

  for (int i : view<reverse_iterator<decltype(is.begin())>>(
           reverse_iterator<decltype(is.begin())>(is.end()),
           reverse_iterator<decltype(is.begin())>(is.begin())))
  {
    cout.printfln(" reverse {}", i);
  }
}

void test_map_iterator()
{
  fixed_vector<int> is(upto(0, 10));
  for (int i : map_view(is.begin(), is.end(), [](int i)
                        { return i + 2; }))
  {
    cout.printfln("map to : {}", i);
  }
}

void test_filter_iterator()
{
  fixed_vector<int> is(upto(0, 10));
  for (int i : filter_view(is.begin(), is.end(), [](const int &i)
                           { return i % 2 == 0; }))
  {
    cout.printfln(" odd {}", i);
  }
}

int main()
{
  test_transfer();
  test_fixed_vector();
  test_copy_algorithm();
  test_move_view();
  test_reverse_iterator();
  test_map_iterator();
  test_filter_iterator();

  return 0;
}
