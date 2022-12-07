// #include <dbfile.hpp>

#include <stew.hpp>

int main()
{
  using namespace stew;
  using namespace stew::fs::literatals;

  // auto tmp = "tmp"_cfp;
  // auto tmp2 = "tmp2"_cfp;

  // cout.printfln("tmp existing ? {}", (bool)fs::fexists(tmp));
  // cout.printfln("tmp created ? {}", (bool)fs::fcreate(tmp));
  // cout.printfln("tmp readable ? {}", (bool)fs::freadable(tmp));
  // cout.printfln("tmp removed ? {}", (bool)fs::fremove(tmp));
  // cout.printfln("tmp created ? {}", (bool)fs::fcreate(tmp));
  // cout.printfln("tmp renamed ? {}", (bool)fs::frename(tmp, tmp2));
  // cout.printfln("tmp removed ? {}", (bool)fs::fremove(tmp2));

  vector<int> fv{10};

  for (size_t i{0}; i < 100; ++i)
  {
    fv.push_back(i);
  }

  for (size_t i{0}; i<fv.size(); ++i)
  {
    cout.printfln(" item {}", fv[i]);
  }

  return fv.size() - 100;
}