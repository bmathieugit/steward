#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  // bien passons au chose serieuse. 
  // Nous alloons maintenant mettre en place un petit 
  // prototype de ce que sera l'outil steward en ligne 
  // de commande. Il devra permettre de sauver des mots
  // de passe et de les redonner sur demande. 
  // Il nous faut donc un algorithm de chiffrement et
  // de déchiffrement pour les stocker de manière la plus
  // sécurisée possible. 
  // Cette algorithme sera à bijectif sur la base d'un 
  // mot de passe maitre. 
  //
  //


  static_string<char, 10> mdp = "password"_sv;
  static_string<char, 100> master = "master"_sv;

  // J'ai deux string l'une qui est le mdp a chiffer et l'autre 
  // qui est le mot de passe maitre clé du chiffrement.
  //

  // On va faire un chiffrement simlement pour commencer. 
  // On va prendre simplement chaque lettre du mot de passe maitre
  // et additionner ce caracter avec le caractère à chiffrer dans 
  // le mot de passe fils.
  //

  vector<byte_t> mdpb = mdp;
  vector<byte_t> masterb = master;
  vector<byte_t> result;

  for (byte_t b : mdpb)
  {
    size_t l = b;

    for (byte_t m : masterb)
    {
      l += m;
    }

    for (byte_t m : *reinterpret_cast<array<byte_t, sizeof(size_t)>*>(&l))
    {
      result.push(m);
    }
  }
  
  string<char> s = result;


  console<char>::printfln("result is '\0'", s);

  

  return 0;
}


