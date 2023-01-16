// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  fixed_string<char> s(100);
  s.push("coucou1412c211"_sv);


  auto res = format_from<int, int>(s, "coucou{}c{}"_sv);

  if (get<0>(res).has())
  {
    printf("%d\n", *get<0>(res));
    printf("%d\n", *get<1>(res));
  }
  else
  {
    printf("not found!\n");
  }

  return 0;
}
