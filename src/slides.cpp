#include <stew.hpp>

using namespace stew;

namespace ansi {
constexpr auto clear = str::view("\033[2J");

constexpr auto init = str::view("\033[H");

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

enum class line_type {};

namespace slide {
class line {
 public:
  enum class type { t1, t2, t3, t4, p, ns, unknown };

 private:
  type translate_prefix_to_line_type(string_view<char> pref) {
    if (starts_with(pref, prefix::ns)) {
      return type::ns;
    } else if (starts_with(pref, prefix::p)) {
      return type::p;
    } else if (starts_with(pref, prefix::t1)) {
      return type::t1;
    } else if (starts_with(pref, prefix::t2)) {
      return type::t2;
    } else if (starts_with(pref, prefix::t3)) {
      return type::t3;
    } else if (starts_with(pref, prefix::t4)) {
      return type::t4;
    } else {
      return type::unknown;
    }
  }

 private:
  type _type = type::unknown;
  static_string<char, 1024> _data;

 public:
  ~line() = default;

  line(string_view<char> data) : _type(translate_prefix_to_line_type(data)) {
    if (_type != type::unknown) {
      _data.push(str::subv(data, 3));
    }
  }

  line(type tp, string_view<char> data) : _type(tp), _data(data) {}

  line(const line&) = default;
  line(line&&) = default;
  line& operator=(const line&) = default;
  line& operator=(line&&) = default;

 public:
  type tp() const { return _type; }
  string_view<char> data() const { return string_view<char>(_data); }
};

struct style;

}  // namespace slide

namespace terminal {
void clear() { termout.push(ansi::clear); }

void init() { termout.push(ansi::init); }

namespace bg {
void red() { termout.push(ansi::bg_red); }
}  // namespace bg

namespace fg {
void bold() { termout.push(ansi::bold); }

void red() { termout.push(ansi::fg_red); }
}  // namespace fg
}  // namespace terminal
// un style est une définition de style pour un type de ligne donnée.
// eg : t1=:31,1

template <size_t N>
bool getline(static_string<char, N>& buff, file<char, mode::r>& f) {
  if (!buff.empty()) {
    return false;
  } else {
    maybe<char> tmp;
    auto reader = f.reader();

    while ((tmp = reader.pop()).has()) {
      buff.push(*tmp);
      if (*tmp == '\n') break;
    }

    if (!tmp.has()) return false;

    return true;
  }
}

void clear_term() { termout.push(ansi::clear); }

void origin_cursor() { termout.push(ansi::init); }

void on_new_slide(slide::line l) {
  termin.pop();
  clear_term();
  origin_cursor();
}

void on_paragraph(slide::line l) { termout.push(l.data()); }

void on_title1(slide::line l) {
  terminal::fg::red();
  terminal::fg::bold();
  termout.push(l.data());
  termout.push(str::view("\033[0m"));
}

void on_title2(slide::line l) {
  termout.push(str::view("\033[1;32m"));
  termout.push(l.data());
  termout.push(str::view("\033[0m"));
}

void on_title3(slide::line l) {
  termout.push(str::view("\033[1;33m"));
  termout.push(l.data());
  termout.push(str::view("\033[0m"));
}

void on_title4(slide::line l) {
  termout.push(str::view("\033[1;34m"));
  termout.push(l.data());
  termout.push(str::view("\033[0m"));
}

int main(int argc, char** argv) {
  if (argc == 2) {
    static_string<char, 1024> buff;
    file<char, mode::r> slides(str::view(argv[1]));

    while (getline(buff, slides)) {
      slide::line l(buff);

      switch (l.tp()) {
        case slide::line::type::ns:
          on_new_slide(l);
          break;
        case slide::line::type::p:
          on_paragraph(l);
          break;
        case slide::line::type::t1:
          on_title1(l);
          break;
        case slide::line::type::t2:
          on_title2(l);
          break;
        case slide::line::type::t3:
          on_title3(l);
          break;
        case slide::line::type::t4:
          on_title4(l);
          break;
        case slide::line::type::unknown:
        default:
          break;
      }

      buff.clear();
    }
  }

  return 0;
}
