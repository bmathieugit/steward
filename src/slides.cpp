#include <stew/iofile.hpp>
#include <stew/iterator.hpp>
#include <stew/maybe.hpp>
#include <stew/string.hpp>
#include <stew/view.hpp>

using namespace stew;

namespace ansi {

constexpr auto clear = str::view("\033[2J");
constexpr auto init = str::view("\033[H");
constexpr auto reset = str::view("\033[0m");
constexpr auto bold = str::view("\033[1m");
constexpr auto bg_red = str::view("\033[41m");
constexpr auto fg_red = str::view("\033[31m");
}  // namespace ansi

namespace prefix {

constexpr auto t1 = str::view("t1=");
constexpr auto t2 = str::view("t2=");
constexpr auto t3 = str::view("t3=");
constexpr auto t4 = str::view("t4=");
constexpr auto p = str::view("p==");
constexpr auto ns = str::view("===");

}  // namespace prefix

int main(int argc, char** argv) {
  if (argc == 2) {
    file<char, mode::r> fslides(argv[1]);

    if (fslides.opened()) {
      auto content = io::readall(fslides);
      auto nb_lines = count(content.iter(), '\n');
      auto ilines = split_iterator(content.iter(), '\n');

      while (ilines.has_next()) {
        auto iline = ilines.next();
        auto prfx = counter_iterator(iline, 3);

        if (equals(prfx, prefix::ns.iter())) {
          io::stdr.pop();
          io::print(ansi::clear);
          io::print(ansi::init);
        } else if (equals(prfx, prefix::t1.iter())) {
          io::println(str::view("t1 !"));
        } else if (equals(prfx, prefix::t2.iter())) {
          io::println(str::view("t2 !"));
        } else if (equals(prfx, prefix::t3.iter())) {
          io::println(str::view("t3 !"));
        } else if (equals(prfx, prefix::t4.iter())) {
          io::println(str::view("t4 !"));
        } else if (equals(prfx, prefix::p.iter())) {
          io::println(str::view("p !"));
        }
      }
    }
  }

  return 0;
}
