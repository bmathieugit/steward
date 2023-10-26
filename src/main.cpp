#include <core/algorithm.hpp>
#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <core/string.hpp>
#include <vault/crypto/ares.hpp>
#include <vault/crypto/base64.hpp>

int main() {
  auto mess = cstring(
      "bonjour tout le monde comment ca va?bonjour tout le monde comment ca "
      "va?bonjour tout le monde comment ca va?bonjour tout le monde comment ca "
      "va?bonjour tout le monde comment ca va?");
  auto key = istream("ceci est ma clé");
  auto ares = vault::crypto::ares(key);
  auto crypted = ares.crypt(mess);
  auto uncrypted = ares.decrypt(crypted);

  sout << "crypted " << vault::crypto::base64(crypted) << '\n';
  sout << "uncrypted " << uncrypted << '\n';

  sout << sizeof(vault::crypto::ares) << '\n';
  // sout << min_of<u32> << '\n';
  sout << '[' << min_of<i32> << ',' << max_of<i32> << ']' << '\n';
  sout << '[' << min_of<u32> << ',' << max_of<u32> << ']' << '\n';
}
