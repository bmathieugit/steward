// #include <dbfile.hpp>

#include <stew.hpp>
#include <threads.h>

void task(stew::string_view id)
{
  for (int i{0}; i < 100; i++)
  {
    stew::cout.printfln("{} : {}", id, i);
    stew::this_thread::sleep(10000, stew::duration_type::nanosecond);
  }
}

int main()
{
  stew::jthread t1([] { task("Benjamin"); });
  stew::jthread t2([] { task("Bob"); });

  return EXIT_SUCCESS;
}