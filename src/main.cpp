// #include <dbfile.hpp>

#include <stew.hpp>

struct toto
{
  int task()
  {
    for (int i{0}; i < 100; i++)
    {
      stew::cout.printfln("{} : {}", i);
      stew::this_thread::sleep<stew::duration_type::second>(10);
    }

    return 0;
  }
};

int main()
{

  toto
  stew::function<int()> f2 = &(toto{}.task;
  { return 12; };
  stew::cout.printfln("{}", f2());

  return EXIT_SUCCESS;
}