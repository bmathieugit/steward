#include <stew.hpp>

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

void on_new_slide(stew::string_view<char> line) {
  stew::termin.pop();
  clear_term();
  origin_cursor();
}

stew::string_view<char> escape_paragraph_prefix(stew::string_view<char> line) {
  return stew::str::subv(line, 1);
}

void on_paragraph(stew::string_view<char> line) {
  stew::termout.push(escape_paragraph_prefix(line));
}

stew::string_view<char> escape_title_prefix(stew::string_view<char> line) {
  return stew::str::subv(line, 1);
}

void on_title1(stew::string_view<char> line) {
  stew::termout.push(stew::str::view("\033[1;31m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

void on_title2(stew::string_view<char> line) {
  stew::termout.push(stew::str::view("\033[1;32m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

void on_title3(stew::string_view<char> line) {
  stew::termout.push(stew::str::view("\033[1;33m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

void on_title4(stew::string_view<char> line) {
  stew::termout.push(stew::str::view("\033[1;34m"));
  stew::termout.push(escape_title_prefix(line));
  stew::termout.push(stew::str::view("\033[0m"));
}

enum class line_type { t1, t2, t3, t4, p, ns, unknown };

line_type get_line_type(stew::string_view<char> line) {
  if (stew::starts_with(line, stew::str::view("t1"))) {
    return line_type::t1;
  } else if (stew::starts_with(line, stew::str::view("t2"))) {
    return line_type::t2;
  } else if (stew::starts_with(line, stew::str::view("t3"))) {
    return line_type::t3;
  } else if (stew::starts_with(line, stew::str::view("t4"))) {
    return line_type::t4;
  } else if (stew::starts_with(line, stew::str::view("p"))) {
    return line_type::p;
  } else if (stew::starts_with(line, stew::str::view("======"))) {
    return line_type::ns;
  }

  return line_type::unknown;
}

int main(int argc, char** argv) {
  if (argc == 2) {
    stew::file<char, stew::mode::r> f(stew::str::view(argv[1]));
    stew::fixed_string<char> buffer(1024);

    while (getline(buffer, f)) {
      stew::string_view<char> line(buffer);

      switch (get_line_type(line)) {
        case line_type::ns:
          on_new_slide(line);
          break;
        case line_type::p:
          on_paragraph(line);
          break;
        case line_type::t1:
          on_title1(line);
          break;
        case line_type::t2:
          on_title2(line);
          break;
        case line_type::t3:
          on_title3(line);
          break;
        case line_type::t4:
          on_title4(line);
          break;
        case line_type::unknown:
        default:
          break;
      }

      buffer.clear();
    }
  }

  return 0;
}
