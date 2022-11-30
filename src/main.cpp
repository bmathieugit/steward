// #include <dbfile.hpp>

#include <stew.hpp>

int main()
{
  using namespace stew;
  fs::directory<void> tmp("tmp");

  tmp.create();
  cout.printfln("tmp exists ? {}", tmp.exists());
  cout.printfln("tmp removed ? {}", tmp.remove());
  cout.printfln("tmp created ? {}", tmp.create());
  cout.printfln("tmp renamed ? {}", tmp.rename("tmp2"));
  tmp.listdirs();
  return 0;
}