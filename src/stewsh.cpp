#include <stew/algorithm.hpp>
#include <stew/iofile.hpp>
#include <stew/iterator.hpp>
#include <stew/string.hpp>
#include <stew/tuple.hpp>
#include <stew/vector.hpp>

// je vais faire un interpreteur de commande perso pour steward.
// il n'y aura que deux type pris en charge : les string et
// les nombres et les boolean (int float bool).
// Il y aura également la possibilité de faire des if et des while.

using namespace stew;

using variable = tuple<string<char>, string<char>>;
using variables = ext_vector<variable>;

template <iterator I>
void process_let(I iline, variables& vars) {
  // la ligne commence par let.
  // on peut donc extraire le nom de variable et l'expression associée

  auto candidate = keep_until(skip(iline, str::view("let").iter()));

  find(vars.iter(), [&candidate](const variable& var) {
    return equals(get<0>(var).iter(), candidate.iter());
  });
}

int main(int argc, char** argv) {
  variables vars;

  if (argc >= 2) {
    // on va commencer par faire de la lecture ligne par ligne d'un script.
    // le premier argument sera le nom du fichier script.

    file<char, mode::r> script(argv[1]);
    string<char> content = io::readall(script);
    auto ilines = split_iterator(content.iter(), '\n');

    for_each(ilines, [](const auto& iline) {
      io::printfln(str::view("read > $"), iline);
    });

  } else {
    // on n'a pas de fichier à parser en tant que script.
    // On va donc s'attendre à ce que le user entre le script
    // au fur et à mesure dans le terminal

    bool not_end = true;

    while (not_end) {
      io::print(str::view("script > "));
      auto line = io::readline(io::stdr);

      if (starts_with(line.iter(), str::view("exit").iter())) {
        return 0;
      } else if (starts_with(line.iter(), str::view("let").iter())) {
        process_let(line.iter(), vars);
      }
    }
  }

  return 0;
}
