#include <stew.hpp>

using namespace stew;

// Ce que je vais faire maintenant c'est de développer un système de gestion des évenements
// le plus basique possible afin de rester aussi près que possible des performances optimales.
// Nous allons partir sur une API type signal/slot sur des types strict. L'héritage ne
// sera pas fonctionnel sur les évènements.


void foo(const int &i)
{
  printf("coucou\n");
}

struct foo2
{
  void operator()(const int &i)
  {
    printf("coucou2\n");
  }
};

int main()
{
  using oi = observer<int>;

  observer<int> i = 12;

  i.slot<oi::signal::changed>([](const int& i) {
    console<char>::printfln("coucou3{}{}", 12, true);
  });

  i = 24;

  return 0;
}
