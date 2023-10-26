#ifndef __stew_vault_crypto_ares_hpp__
#define __stew_vault_crypto_ares_hpp__

#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/string.hpp>
#include <vault/crypto/mash.hpp>

namespace vault::crypto {

class ares {
 private:
  array<array<char, 64>, 64> _keys;

 public:
  constexpr ares(char_input_stream auto key) {
    _keys.add(mash{}.digest(key));

    for (size_t i = 1; i < 64; ++i) {
      _keys.add(mash{}.digest(istream(_keys.at(i - 1))));
    }
  }

 public:
  constexpr fixed_string crypt(const char_collection auto& mess) const {
    fixed_string crypted(mess);
    auto im = istream(_keys);

    while (im.has()) {
      auto&& krow = im.next();
      auto ordi = crypted.ord();

      while (ordi.has()) {
        auto i = ordi.next();
        auto c = crypted.at(i);
        auto ckey = krow.at(i % 64);
        crypted.modify(i, c ^ ckey);
      }
    }

    return crypted;
  }

  constexpr fixed_string decrypt(const char_collection auto& mess) const {
    fixed_string crypted(mess);
    auto rim = ristream(_keys);

    while (rim.has()) {
      auto&& krow = rim.next();
      auto ordi = crypted.ord();

      while (ordi.has()) {
        auto i = ordi.next();
        auto c = crypted.at(i);
        auto ckey = krow.at(i % 64);
        crypted.modify(i, c ^ ckey);
      }
    }

    return crypted;
  }
};

}  // namespace vault::crypto

#endif