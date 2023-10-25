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
  constexpr fixed_string crypt(const char_collection auto& key,
                               const char_collection auto& mess) const {
    fixed_string keycp(key);
    fixed_string crypted(mess);

    for (size_t i = 0; i < crypted.len(); ++i) {
      auto c = crypted.at(i);
      auto ckey = keycp.at(i % keycp.len());
      auto cc = c ^ ckey;
      crypted.modify(i, cc);
    }

    return shuffle(crypted,
                   random_index_input_stream(crypted.len(), crypted.len()));
  }

  constexpr fixed_string decrypt(const char_collection auto& key,
                                 const char_collection auto& mess) const {
    return crypt(key, mess);
  }
};

}  // namespace vault::crypto

#endif