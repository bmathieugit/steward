// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

int main()
{
  string_view<char> hello = "hello 12020 12021"_sv;
  format_response<int, int> res;

  format_from(hello, "hello {} {}"_sv, res);

  if (get<0>(res).has() && get<1>(res).has())
  {
    format_to(termout, "{}, {}"_sv, *get<0>(res), *get<1>(res));
    // printf("%d, %d", *get<0>(res), *get<1>(res));
  }

  return 0;
}
