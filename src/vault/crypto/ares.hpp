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
      const auto& ikey = _keys.at(i - 1);
      auto b = begin(ikey);
      auto e = end(ikey);
      _keys.modify(mash{}.digest(b, e), i);
    }
  }

 public:
  constexpr string crypt(string_view mess) const {
    string crypted(mess);

    printf("Length of mess %lu\n", mess.len());

    for (size_t i = 0; i < _keys.len(); ++i) {
      auto&& krow = _keys.at(i);

      for (size_t j = 0; j < crypted.len(); ++j) {
        auto c = crypted.at(j);
        auto ckey = krow.at(c % krow.len());
        crypted.modify(c ^ ckey, j);
      }
    }

    return crypted;
  }

  constexpr string decrypt(string_view mess) const {
    string crypted(mess);

    printf("Length of mess %lu\n", mess.len());

    for (size_t i = _keys.len(); i-- > 0;) {
      auto&& krow = _keys.at(i);

      for (size_t j = 0; j < crypted.len(); ++j) {
        auto c = crypted.at(j);
        auto ckey = krow.at(c % krow.len());
        crypted.modify(c ^ ckey, j);
      }
    }

    return crypted;
  }
};

}  // namespace vault::crypto

#endif