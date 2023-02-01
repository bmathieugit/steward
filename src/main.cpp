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

    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
    files<char>::printfln(writer, "un jolie phrase{}", 0);
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
    files<char>::printfln(writer, "un jolie phrase{}", 1);
  }

  dialog<char> diag;

  extract_response<int> response;
  diag.println("quel est ton age ?")
      .readf("{}", response)
      .printfln("tu as dit avoir {} an(s)", response.get<0>().operator*());

  return 0;
}
