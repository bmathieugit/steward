#include <iostream>
#include <string>

#include <core/string.hpp>

string chiffrementAres(const string& cle, char_iterator auto texte) {
  string texte_chiffre;
  int length = cle.len();

  while (texte.has()) {
    char octet_a_chiffrer = texte.get();
    char cle_octet = cle.at(texte.pos() % length);
    char octet_chiffre = octet_a_chiffrer + cle_octet;
    texte_chiffre.add(octet_chiffre);
    texte.next();
  }

  return texte_chiffre;
}

int main() {
  auto cle = iter("votre_cle");  // Remplacez "votre_cle" par la clé souhaitée
  auto texte = iter("Le texte que vous souhaitez chiffrer.");

  string texte_chiffre = chiffrementAres(cle, texte);

  // cout << "Texte chiffré : " << texte_chiffre << endl;
  // cout << "Texte déchiffré : " << texte_dechiffre << endl;

  return 0;
}
