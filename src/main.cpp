#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <core/string.hpp>
#include <vault/crypto/ares.hpp>
#include <vault/crypto/mash.hpp>

int main() {
  constexpr vault::crypto::mash mash;
  constexpr vault::crypto::ares ares;
  auto mess = istream("bonjour tout le monde comment ca va?");
  sout << mash.digest(mess) << '\n';
  auto crypted = ares.crypt(istream(mash.digest(mess)), mess);
  auto uncrypted = ares.crypt(istream(mash.digest(mess)), istream(crypted));
  sout << "crypted " << base64(crypted) << '\n';
  sout << "uncrypted " << uncrypted << '\n';
}
