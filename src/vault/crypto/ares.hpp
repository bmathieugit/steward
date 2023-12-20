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

    for (size_t i = 0; i < crypted.len(); ++i) {
      auto c_crypt = crypted.at(i);

      for (size_t j = 0; j < _keys.len(); ++j) {
        auto c_key = _keys.at(j).at(i % 64);
        c_crypt = c_crypt ^ c_key;
      }

      crypted.modify(c_crypt, i);
    }

    auto middle = crypted.len() / 2;

    for (size_t i = 0; i < middle; ++i) {
      crypted.exchange(i, crypted.len() - i - 1);
    }

    return crypted;
  }

  constexpr string decrypt(string_view mess) const {
    string crypted(mess);

    auto middle = crypted.len() / 2;

    for (size_t i = 0; i < middle; ++i) {
      crypted.exchange(i, crypted.len() - i - 1);
    }

    for (size_t i = 0; i < crypted.len(); ++i) {
      auto c_crypt = crypted.at(i);

      for (size_t j = _keys.len(); j-- > 0;) {
        auto c_key = _keys.at(j).at(i % 64);
        c_crypt = c_crypt ^ c_key;
      }

      crypted.modify(c_crypt, i);
    }

    return crypted;
  }
};

}  // namespace vault::crypto

#endif