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

    return true;
  }
}

int main(int argc, char** argv) {
  if (argc == 2) {
    stew::file<char, stew::mode::r> f(stew::str::view(argv[1]));
    stew::fixed_string<char> buffer(1024);

    while (getline(buffer, f)) {
      stew::console<char>::printfln(stew::str::view("line read : \0"), buffer);

      if (!stew::starts_with(buffer, stew::str::view("======"))) {
        stew::termout.push(buffer);
      }

      buffer.clear();
    }
  }

  return 0;
}
