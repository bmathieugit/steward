#ifndef __stew_vault_crypto_ares_hpp__
#define __stew_vault_crypto_ares_hpp__

#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/string.hpp>

template <integral I>
constexpr I rotl(I i, int n) {
  return (i << (n % (sizeof(I) * 8))) |
         (i >> (sizeof(I) - (n % (sizeof(I) * 8))));
}

template <integral I>
constexpr I rotr(I i, int n) {
  return (i >> (n % (sizeof(I) * 8))) |
         (i << (sizeof(I) - (n % (sizeof(I) * 8))));
}

namespace vault::crypto {

class ares {
 public:
  constexpr string crypt(char_input_stream auto key,
                         char_input_stream auto is) const {
    string keycp(key);
    string crypted(is);

    for (size_t i = 0; i < crypted.len(); ++i) {
      auto c = crypted.at(i);
      auto ckey = keycp.at(i % keycp.len());
      auto cc = c ^ ckey;
      crypted.modify(i, cc);
    }

    return crypted;
  }

  constexpr string decrypt(char_input_stream auto key,
                           char_input_stream auto is) const {
    return crypt(key, is);
  }
};

}  // namespace vault::crypto

#endif