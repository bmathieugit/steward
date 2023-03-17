#include <stew.hpp>

constexpr auto t1_prefix = stew::str::view("t1=");
constexpr auto t2_prefix = stew::str::view("t2=");
constexpr auto t3_prefix = stew::str::view("t3=");
constexpr auto t4_prefix = stew::str::view("t4=");
constexpr auto p_prefix = stew::str::view("p=");
constexpr auto ns_prefix = stew::str::view("===");

enum class line_type { t1, t2, t3, t4, p, ns, unknown };

line_type translate_prefix_to_line_type(stew::string_view<char> pref) {
  if (stew::starts_with(pref, ns_prefix)) {
    return line_type::ns;
  } else if (stew::starts_with(pref, p_prefix)) {
    return line_type::p;
  } else if (stew::starts_with(pref, t1_prefix)) {
    return line_type::t1;
  } else if (stew::starts_with(pref, t2_prefix)) {
    return line_type::t2;
  } else if (stew::starts_with(pref, t3_prefix)) {
    return line_type::t3;
  } else if (stew::starts_with(pref, t4_prefix)) {
    return line_type::t4;
  } else {
    return line_type::unknown;
  }
}

class line {
 private:
  line_type _type = line_type::unknown;
  stew::fixed_string<char> _data = stew::fixed_string<char>(1024);

 public:
  ~line() = default;
  
  line(stew::fixed_string<char> l) : _type(translate_prefix_to_line_type(l)) {
    if (_type != line_type::unknown) {
      stew::string_view<char> tmp(l);
      tmp = stew::str::subv(tmp, 3);
      _data.push(tmp);
    }
  }
  
  line(line_type type, stew::fixed_string<char> data)
      : _type(type), _data(stew::transfer(data)) {}
  
  line(const line&) = default;
  line(line&&) = default;
  line& operator=(const line&) = default;
  line& operator=(line&&) = default;

 public:
  line_type type() const { return _type; }
  stew::string_view<char> data() const {
    return stew::string_view<char>(_data);
  }
};

bool getline(stew::fixed_string<char>& buff,
             stew::file<char, stew::mode::r>& f) {
  if (!buff.empty()) {
    return false;
  } else {
    stew::maybe<char> tmp;
    auto reader = f.reader();

    while ((tmp = reader.pop()).has()) {
      buff.push(*tmp);
      if (*tmp == '\n') break;
    }

    if (!tmp.has()) return false;

    return true;
  }
}

void clear_term() { stew::termout.push(stew::str::view("\033[2J")); }

void origin_cursor() { stew::termout.push(stew::str::view("\033[H")); }

void on_new_slide(line l) {
  stew::termin.pop();
  clear_term();
  origin_cursor();
}

stew::string_view<char> escape_paragraph_prefix(stew::string_view<char> line) {
  return stew::str::subv(line, 1);
}

void on_paragraph(line l) { stew::termout.push(escape_paragraph_prefix(line)); }

stew::string_view<char> escape_title_prefix(stew::string_view<char> line) {
  return stew::str::subv(line, 1);
}

void on_title1(line l) {
  stew::termout.push(stew::str::view("\033[1;31m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

void on_title2(line l) {
  stew::termout.push(stew::str::view("\033[1;32m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

void on_title3(line l) {
  stew::termout.push(stew::str::view("\033[1;33m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

void on_title4(line l) {
  stew::termout.push(stew::str::view("\033[1;34m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

int main(int argc, char** argv) {
  if (argc == 3) {
    stew::file<char, stew::mode::r> slides(stew::str::view(argv[1]));
    stew::file<char, stew::mode::r> theme(stew::str::view(argv[1]));

    line l;

    while (getline(l, slides)) {
      stew::string_view<char> lview(l._data);

      switch (l._type) {
        case line_type::ns:
          on_new_slide(l);
          break;
        case line_type::p:
          on_paragraph(l);
          break;
        case line_type::t1:
          on_title1(l);
          break;
        case line_type::t2:
          on_title2(l);
          break;
        case line_type::t3:
          on_title3(l);
          break;
        case line_type::t4:
          on_title4(l);
          break;
        case line_type::unknown:
        default:
          break;
      }
    }
  }

  return 0;
}
