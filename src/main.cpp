#include <stew.hpp>

using namespace stew;

int main()
{

  atomic<int> a(20);

  jthread th1([&a]()
              { for (int j : upto(0, 1000))
                a.apply([](int &i)
                        {
                  console<char>::printfln("th1 {}",++i); }); });

  jthread th0([&a]()
              { for (int j : upto(0, 1000))
              a.apply([](int &i)
                        { 
                          console<char>::printfln("th0 {}", ++i); }); });
  return 0;
}
