#ifndef __stew_vault_crypto_base64_hpp__
#define __stew_vault_crypto_base64_hpp__

#include <core/core.hpp>
#include <core/string.hpp>

namespace text {
auto base64(const char_collection auto& s) {
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
}  // namespace text

#endif