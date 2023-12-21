#ifndef __stew_vault_crypto_base64_hpp__
#define __stew_vault_crypto_base64_hpp__

#include <core/array.hpp>
#include <core/core.hpp>
#include <core/string.hpp>

namespace text::base64 {
constexpr auto encode(string_view s) {
  constexpr char k[] =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  const size_t len = s.len();
  size_t i = 0;

  string encoded(((len + 2) / 3 * 4) + 1);

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

constexpr auto __decode_table() {
  array<int, 256> table = {};

  for (int i = 0; i < 64; i++) {
    table.modify(
        i,
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]);
  }

  return table;
}

constexpr string decode(string_view encoded) {
  // Tableau pour le mappage inverse des caractères de Base64
  constexpr auto table = __decode_table();
  // Vérifier la longueur de la chaîne encodée
  if (encoded.len() % 4 != 0) {
    throw "";
  }

  // Calcul de la taille de la chaîne décodée
  size_t outputLength = encoded.len() / 4 * 3;

  if (encoded.at(encoded.len() - 1) == '=') {
    outputLength--;
    if (encoded.at(encoded.len() - 2) == '=') {
      outputLength--;
    }
  }

  string decoded(outputLength);

  // Processus de décodage
  for (size_t i = 0; i < encoded.len();) {
    int a = table.at(encoded.at(i++));
    int b = table.at(encoded.at(i++));
    int c = table.at(encoded.at(i++));
    int d = table.at(encoded.at(i++));

    decoded.add((a << 2) | (b >> 4));

    if (c != 0) {
      decoded.add(((b & 15) << 4) | (c >> 2));

      if (d != 0) {
        decoded.add(((c & 3) << 6) | d);
      }
    }
  }

  return decoded;
}
}  // namespace text::base64

#endif