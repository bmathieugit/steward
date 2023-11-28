#ifndef __stew_vault_crypto_ares_hpp__
#define __stew_vault_crypto_ares_hpp__

#include <core/string.hpp>
#include <vault/crypto/mash.hpp>

namespace vault::crypto {

class ares {
 private:
  static_vector<static_vector<char, 64>, 64> _keys;

 public:
  constexpr ares(char_iterator auto key) {
    _keys.add(mash{}.digest(key));

    for (size_t i = 1; i < 64; ++i) {
      _keys.add(mash{}.digest(iter(_keys.at(i - 1))));
    }
  }

 public:
  constexpr fixed_string crypt(const char_collection auto& mess) const {
    fixed_string crypted(mess);
    auto im = iter(_keys);

    while (im.has()) {
      auto&& krow = im.next();
      auto ccrypted = iter(crypted);

      while (ccrypted.has()) {
        auto i = ccrypted.next();
        auto c = crypted.at(i);
        auto ckey = krow.at(i % 64);
        crypted.modify(c ^ ckey, i);
      }
    }

    return crypted;
  }

  constexpr fixed_string decrypt(const char_collection auto& mess) const {
    fixed_string crypted(mess);
    auto rim = riter(_keys);

    while (rim.has()) {
      auto&& krow = rim.next();
      auto ccrypted = iter(crypted);

      while (ccrypted.has()) {
        auto i = ccrypted.next();
        auto c = crypted.at(i);
        auto ckey = krow.at(i % 64);
        crypted.modify(c ^ ckey, i);
      }
    }

    return crypted;
  }
};

}  // namespace vault::crypto

#endif