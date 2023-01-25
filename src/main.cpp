#include <stew.hpp>

using namespace stew;

int main()
{
  atomic<int> a(20);

  jthread([&a]()
          { for (int j : upto(0, 1000))
                a.apply([](int &i)
                        {
                  console<char>::printfln("th1 {}",++i); }); });

  jthread([&a]()
          { for (int j : upto(0, 1000))
              a.apply([](int &i)
                        { 
                          console<char>::printfln("th0 {}", ++i); }); });

  return 0;
}
