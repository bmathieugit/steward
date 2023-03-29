#define NDEBUG

#include <stew/string.hpp>
#include <stew/iofile.hpp>

using namespace stew;

int main(int argc, char** argv) {
  {
    file<int, mode::w> fw(str::view("ftest.t"));
    file<int, mode::rp> fw2(str::view("ftest2.t"));
    
    if (fw.opened()) {
      fw.writer().push(1);
      fw.writer().push(2);
      fw.writer().push(3);
    }
  }

  {
    file<int, mode::r> fr(str::view("ftest.t"));

    if (fr.opened()) {
      auto content = io::readall(fr);
      io::printfln(str::view("\0"), io::len(fr));
      io::printfln(str::view("\0"), view(content));
    }
  }

  return 0;
}
