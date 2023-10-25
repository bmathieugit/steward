#include <core/algorithm.hpp>
#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <core/string.hpp>
#include <vault/crypto/ares.hpp>
#include <vault/crypto/base64.hpp>
#include <vault/crypto/mash.hpp>

int main() {
  constexpr vault::crypto::ares ares;

  auto mess = cstring(
      "bonjour tout le monde comment ca va?bonjour tout le monde comment ca "
      "va?bonjour tout le monde comment ca va?bonjour tout le monde comment ca "
      "va?bonjour tout le monde comment ca va?");
  auto key = cstring("ceci est ma clé");

  auto crypted = ares.crypt(key, mess);
  auto uncrypted = ares.decrypt(key, crypted);

  sout << "crypted " << vault::crypto::base64(crypted) << '\n';
  sout << "uncrypted " << uncrypted << '\n';

  
}
