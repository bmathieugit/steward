#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  int i = -12;
  console<char>::printfln("{}", i);

  {
    file<char, mode::w> out("./test.txt"_sv);
    auto writer = out.writer();

    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
    format_to(writer, "un jolie phrase{}\n", 0);
  }

  long pos = 0;

  {
    file<char, mode::r> in("./test.txt"_sv);
    auto reader = in.reader();
    maybe<char> m;

    while ((m = reader.pop()).has() && m.operator*() != '\n')
      ;

    pos = in.tellg();
  }

  {
    file<char, mode::rp> out("./test.txt"_sv);

    auto writer = out.writer();
    out.seekg(pos, seek::set);
    format_to(writer, "un jolie phrase{}\n", 1);
  }

  return 0;
}
