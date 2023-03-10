#include <stew.hpp>

using namespace stew;

// une suite de test est composée d'un nom et d'une liste de test
// Un test est composé d'un nom et d'une fonction de test
// Une fonction de test est un ensemble d'assertions. 
// Une fonction de test retourne un status de test. 
// Le status de test peut être soit OK, soit KO
//
// Apercu d'un rapport typique à obtenir
//
// SUITE [Test de string] *************************************************
//
// TEST [La longueur de la chaine "coucou" sous une vue est 6] ************
// ok:
//
// TEST [La longueur de la chaine "coucou" sous une fixed_string est de 6 ] ****
// ok: 
//
// TEST [La longueur de la chaine "coucou" sous une fixed_string est de 6 ] ****
// ko: la longueur retrouvée est 7. 
// 
//

int main() {
  console<char>::printfln(str::view("view size of 'coucou' : \0"), str::view("coucou").size());
  console<char>::printfln(str::view("fixed_string size of 'coucou' : \0"), str::fixed("coucou").size());
  console<char>::printfln(str::view("string size of 'coucou' : \0"), str::str("coucou").size());

}
