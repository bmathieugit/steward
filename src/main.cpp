// #include <dbfile.hpp>

#include <stew.hpp>

void task(stew::string_view id)
{
  for (int i{0}; i < 100; i++)
  {
    stew::cout.printfln("{} : {}", id, i);
    stew::this_thread::sleep<stew::duration_type::second>(10);
  }
}

int main()
{
  stew::mutex m;
  stew::scoped_lock scoped(m); 
  
  stew::jthread t1([] { task("Benjamin"); });
  stew::jthread t2([] { task("Bob"); });

  return EXIT_SUCCESS;
}