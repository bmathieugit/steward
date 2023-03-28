#include <stew.hpp>

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

namespace slide2 {
struct line {
  fixed_string<char> style;
  fixed_string<char> content;
};

struct style {
  fixed_string<char> prefix;
  fixed_string<char> applied;
};

struct styles {
  static_vector<style, 10> stls;
};

maybe<line> getline(string_view<char> sls) {
  maybe<line> m;
  auto fnd = stew::find(sls, '\n');

  if (fnd != sls.end()) {
    string_view<char> lfnd(sls.begin(), fnd);

    if (lfnd.size() >= 3) {
      fixed_string<char> style(3);
      fixed_string<char> content(lfnd.size());

      style.push(string_view<char>(lfnd.begin(), lfnd.begin() + 3));
      content.push(string_view<char>(lfnd.begin() + 3, lfnd.end()));

      line l = {.style = transfer(style), .content = transfer(content)};

      m = transfer(l);
    }
  }

  return m;
}

string_view<char> eraseline(string_view<char> sls, const line& l) {
  if (sls.size() >= l.content.size() + 3)
    return str::subv(sls, l.content.size() + 4);
  else
    return string_view<char>();
}

fixed_string<char> applystyle(const line& l, const styles& sts) {
  auto fnd =
      find(sts.stls, [&l](const style& s) { return s.prefix == l.style; });

  if (fnd != sts.stls.end()) {
    fixed_string<char> applied((*fnd).applied.size() + l.content.size() + 10);
    applied.push((*fnd).applied);
    applied.push(l.content);
    applied.push(ansi::reset);
    return applied;
  } else {
    fixed_string<char> applied(l.content);
    return applied;
  }
}

}  // namespace slide2

int main(int argc, char** argv) {
  if (argc == 2) {
    file<char, mode::r> fslides(str::view(argv[1]));

    if (fslides.opened()) {
      slide2::styles stls;
      stls.stls.push(slide2::style{prefix::ns
          , str::cat(ansi::clear, ansi::init)});
      stls.stls.push(
          slide2::style{prefix::t1, ansi::bg_red});
      stls.stls.push(
          slide2::style{prefix::t2, ansi::bg_red});
      stls.stls.push(
          slide2::style{prefix::t3, ansi::fg_red});
      stls.stls.push(
          slide2::style{prefix::t4, ansi::fg_red});
      stls.stls.push(
          slide2::style{prefix::p, ansi::bold});

      auto all = io::readall(fslides);
      auto vall = string_view<char>(all);

      maybe<slide2::line> l;

      while ((l = slide2::getline(vall)).has()) {
        io::println(view<const char*>(slide2::applystyle(l, stls)));
        
        if ((*l).style == str::view("==="))
            stew::io::stdfr.pop();
        vall = slide2::eraseline(vall, l);
      }
    }
  }

  return 0;
}
