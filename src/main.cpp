#include <ui.hpp>

#include <thread>
#include <iostream>

namespace ui = stew::ui;

/**
 * Mon prochain but est de faire une application qui 
 * va copier un gros paquet de fichier (virtuellement) 
 * et qui va rafraichir l'affichage avce le nom du fichier qui va etre copié. 
 * on aura aussi un pourcentage du processus de copie global qui se mettra à jour. 
 * 
 * Il me faut donc deux zone de message dans la partie TUI de l'application:  
 *  - une pour accueillir le path courant
 *  - une pour accueillir le % courant en cours.
 * 
 * Il y aura donc un producer et un consumer utilisé dans un thread chacun
 * 
 * Il y aura une view qui aura utilisera le consumer pour obtenir l'information
 * d'avancement et du fichier encours de copie.
 * 
 * Il y aura dans le m^eme temps un acteur qui fera la copie et se servira du 
 * producer pour emettre un event à chaque fichier copié.
 */ 

int main(int argc, char **argv)
{
  ui::bus bus;

  ui::consumer cons(bus, "hello");
  ui::producer prod(bus, "hello");


  std::thread tprod([&prod]
                    {
    int i = 0;

    while(true)
    {
      std::cout << std::string("production") + std::to_string(i) + '\n';
      prod.produce(ui::message{._data = std::to_string(i) + " - Hello World !!"});
      i = i + 1;
    } });

    tprod.join();

  return EXIT_SUCCESS;
}
