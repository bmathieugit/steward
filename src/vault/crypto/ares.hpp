#ifndef __stew_vault_crypto_ares_hpp__
#define __stew_vault_crypto_ares_hpp__

#include <core/array.hpp>
#include <core/string.hpp>
#include <vault/crypto/mash.hpp>

namespace vault::crypto {

class ares {
 private:
  array<array<char, 64>, 64> _keys;

 public:
  constexpr ares(string_view key) {
    _keys.modify(mash{}.digest(key), 0);

    for (size_t i = 1; i < 64; ++i) {
      _keys.modify(mash{}.digest(begin(_keys.at(i - 1)), end(_keys.at(i - 1))),
                   i);
    }
  }

 public:
  constexpr string crypt(string_view mess) const {
    string crypted(mess);
    auto bg = begin(_keys);
    auto ed = end(_keys);

    while (bg != ed) {
      auto&& krow = *(bg++);
      auto bgcrypted = begin(crypted);
      auto edcrypted = end(crypted);

      while (bgcrypted != edcrypted) {
        auto i = *(bgcrypted++);
        auto c = crypted.at(i);
        auto ckey = krow.at(i % krow.len());
        crypted.modify(c ^ ckey, i);
      }
    }

    return crypted;
  }

  constexpr string decrypt(string_view mess) const {
    string crypted(mess);
    auto bg = begin(_keys);
    auto ed = end(_keys);
    bg--;
    ed--;

    while (ed != bg) {
      auto&& krow = *(ed--);
      auto bgcrypted = begin(crypted);
      auto edcrypted = end(crypted);

      while (bgcrypted != edcrypted) {
        auto i = *(bgcrypted++);
        auto c = crypted.at(i);
        auto ckey = krow.at(i % krow.len());
        crypted.modify(c ^ ckey, i);
      }
    }

    return crypted;
  }
};

}  // namespace vault::crypto

#endif