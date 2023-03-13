#include <stew.hpp>

int main(int argc, char** argv) {
  if (argc == 2) {
    stew::string_view<char> fname = stew::str::view(argv[1]);
    stew::string<char> buffer;
    stew::file<char, stew::mode::r> fsls(fname);
    fsls.reader().pop(buffer);
    stew::string_view<char> sls(buffer.begin(), buffer.end());
    
    while (!sls.empty()) {
      if (stew::starts_with(sls, stew::str::view("======"))) {
        sls = stew::str::subv(sls, 6);

        while (!sls.empty() &&
               !stew::starts_with(sls, stew::str::view("======"))) {
          stew::termout.push(*stew::begin(sls));
          sls = stew::str::subv(sls, 1);
        }

        stew::termin.pop();
      }
    }
  }
  return 0;
}
