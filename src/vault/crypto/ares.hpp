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

fixed_string base64(const string& s) {
  constexpr char k[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  const size_t len = s.len();
  size_t i = 0;

  fixed_string encoded(((len + 2) / 3 * 4) + 1);

  for (i = 0; i < len - 2; i += 3) {
    const auto c0 = s.at(i);
    const auto c1 = s.at(i + 1);
    const auto c2 = s.at(i + 2);

    encoded.add(k[(c0 >> 2) & 0x3F]);
    encoded.add(k[((c0 & 0x3) << 4) | ((c1 & 0xF0) >> 4)]);
    encoded.add(k[((c1 & 0xF) << 2) | ((c2 & 0xC0) >> 6)]);
    encoded.add(k[c2 & 0x3F]);
  }

  if (i < len) {
    const auto c0 = s.at(i);

    encoded.add(k[(c0 >> 2) & 0x3F]);

    if (i == (len - 1)) {
      encoded.add(k[((c0 & 0x3) << 4)]);
      encoded.add('=');
    }

    else {
      const auto c1 = s.at(i + 1);
      encoded.add(k[((c0 & 0x3) << 4) | ((c1 & 0xF0) >> 4)]);
      encoded.add(k[((c1 & 0xF) << 2)]);
    }

    encoded.add('=');
  }

  return encoded;
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