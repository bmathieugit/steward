#include <stew/iofile.hpp>
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
constexpr auto p = str::view("p=");
constexpr auto ns = str::view("===");

}  // namespace prefix

struct line {
  string<char> style;
  string<char> content;
};

struct style {
  string<char> prefix;
  string<char> applied;
};

struct styles {
  static_vector<style, 10> stls;
};

int main(int argc, char** argv) {
  if (argc == 2) {
    file<char, mode::r> fslides(argv[1]);

    if (fslides.opened()) {
      auto all = io::readall(fslides);
      auto iall = all.iter();
      static_string<char, 1024> l;

      while (iall.has_next()) {
        auto c = iall.next();
        io::printf(str::view("$"), c);
        /*if (c != '\n') {
          l.push(iall.next());
        } else {
          io::printfln(str::view("line : $"), l);
          l.clear();
        }*/
      }
    }
  }

  return 0;
}
