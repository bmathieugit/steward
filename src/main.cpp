#include <stew.hpp>

using namespace stew;

// Ce que je vais faire maintenant c'est de développer un système de gestion des évenements
// le plus basique possible afin de rester aussi près que possible des performances optimales.
// Nous allons partir sur une API type signal/slot sur des types strict. L'héritage ne
// sera pas fonctionnel sur les évènements.


int main()
{
  atomic<int> i(20);
  i.apply([](int &t) { t = 25; });
  i.apply([](const int &t) { console<char>::printfln("nouvelle valeur : {}", t); });

  return 0;
}
