#include <stew.hpp>

bool getline(stew::fixed_string<char>& buff,
             stew::file<char, stew::mode::r>& f) {
  if (!buff.empty()) {
    return false;
  } else {
    stew::maybe<char> tmp;
    auto reader = f.reader();

    while ((tmp = reader.pop()).has() && *tmp != '\n') {
      buff.push(*tmp);
    }

    if (!tmp.has()) return false;

    return true;
  }
}

void clear_term() { stew::termout.push(stew::str::view("\033[2J")); }

void origin_cursor() { stew::termout.push(stew::str::view("\033[H")); }

bool is_new_slide_line(stew::string_view<char> line) {
  return stew::starts_with(line, stew::str::view("======"));
}

bool is_paragraph_line(stew::string_view<char> line) {
  return stew::starts_with(line, stew::str::view("p"));
}

stew::string_view<char> escape_paragraph_prefix(stew::string_view<char> line) {
  return stew::str::subv(line, 1);
}

int main(int argc, char** argv) {
  if (argc == 2) {
    stew::file<char, stew::mode::r> f(stew::str::view(argv[1]));
    stew::fixed_string<char> buffer(1024);

    while (getline(buffer, f)) {
      stew::string_view<char> line(buffer);

      if (is_new_slide_line(line)) {
        stew::termin.pop();
        clear_term();
        origin_cursor();
      } else if (is_paragraph_line(line)) {
        stew::termout.push(escape_paragraph_prefix(line));
      }

      buffer.clear();
    }
  }

  return 0;
}
