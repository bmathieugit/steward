// #include <dbfile.hpp>

#include <stew.hpp>

int main()
{
  using namespace stew;
  fs::file<void> tmp("tmp");
  fs::file<void> tmp2("tmp2");

  cout.printfln("tmp existing ? {}", (bool)fs::fexists(tmp));
  cout.printfln("tmp created ? {}", (bool)fs::fcreate(tmp));
  cout.printfln("tmp readable ? {}", (bool)fs::freadable(tmp));
  cout.printfln("tmp removed ? {}", (bool)fs::fremove(tmp));
  cout.printfln("tmp created ? {}", (bool)fs::fcreate(tmp));
  cout.printfln("tmp renamed ? {}", (bool)fs::frename(tmp, tmp2));
  // cout.printfln("tmp removed ? {}", (bool)fs::fremove(tmp2));
  // tmp.listdirs();
  return 0;
}