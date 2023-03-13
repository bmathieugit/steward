#include <stew.hpp>

int main() {
  // on va commencer par mettre en place l'affichage d'un enchainement se
  // slides. chaque slide sera séparée par un ensemble prédéfini de caractère :
  // eg : ======
  //
  stew::string_view<char> sls = stew::str::view(
      "======\n ceci est ma première slide\n======\n ceci est ma secode "
      "slide\n");
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
  return 0;
}
