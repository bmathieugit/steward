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

// TODO: a booster en termes de perf
constexpr string base64(const string& data) {
  constexpr char base64Chars[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  string base64;
  size_t i = 0;

  while (i < data.len()) {
    // Première séquence de 6 bits
    base64.add(base64Chars[data.at(i) >> 2]);

    // Deuxième séquence de 6 bits
    if (i + 1 < data.len()) {
      base64.add(
          base64Chars[((data.at(i) & 0x03) << 4) | (data.at(i + 1) >> 4)]);
    } else {
      base64.add(base64Chars[(data.at(i) & 0x03) << 4]);
      base64.add('=');
      base64.add('=');
      break;
    }

    // Troisième séquence de 6 bits
    if (i + 2 < data.len()) {
      base64.add(
          base64Chars[((data.at(i + 1) & 0x0F) << 2) | (data.at(i + 2) >> 6)]);
    } else {
      base64.add(base64Chars[(data.at(i + 1) & 0x0F) << 2]);
      base64.add('=');
      break;
    }

    // Quatrième séquence de 6 bits
    base64.add(base64Chars[data.at(i + 2) & 0x3F]);

    i += 3;
  }

  return base64;
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
    string keycp(key);
    string uncrypted(is);

    for (size_t i = 0; i < uncrypted.len(); ++i) {
      auto c = uncrypted.at(i);
      auto ckey = keycp.at(i % keycp.len());
      auto cc = c ^ ckey;
      uncrypted.modify(i, cc);
    }

    return uncrypted;
  }
};

}  // namespace vault::crypto

#endif