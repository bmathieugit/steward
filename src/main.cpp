#include <core/char-ostream.hpp>
#include <core/io.hpp>
#include <core/string.hpp>
#include <vault/crypto/ares.hpp>
#include <vault/crypto/base64.hpp>
#include <vault/crypto/mash.hpp>

int main() {
  constexpr vault::crypto::mash mash;
  constexpr vault::crypto::ares ares;
  auto mess = istream("bonjour tout le monde comment ca va?");
  auto digest = mash.digest(mess);
  auto&& crypted = ares.crypt(istream(digest), mess);
  auto&& uncrypted = ares.decrypt(istream(digest), istream(crypted));
  sout << "crypted " << vault::crypto::base64(crypted) << '\n';
  sout << "uncrypted " << uncrypted << '\n';
}
