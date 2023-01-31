#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  int i = -12;
  console<char>::printfln("{}", i);

  {
    file<char, mode::w> out("./test.txt"_sv);

    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
    format_to(out, "un jolie phrase{}\n", 0);
  }

  long pos = 0;

  {
    file<char, mode::r> in("./test.txt"_sv);
    maybe<char> m;

    while ((m = in.pop()).has() && m.operator*() != '\n')
      ;

    pos = in.tellg();
  }

  {
    file<char, mode::rp> out("./test.txt"_sv);
    out.seekg(pos, seek::set);
    format_to(out, "un jolie phrase{}\n", 1);  
  }

  return 0;
}
