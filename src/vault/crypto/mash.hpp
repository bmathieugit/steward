#ifndef __stew_crypto_sha_hpp__
#define __stew_crypto_sha_hpp__

#include <core/array.hpp>
#include <core/core.hpp>
#include <core/random.hpp>
#include <core/string.hpp>

namespace vault::crypto {

class mash {
 public:
    constexpr auto digest(string_view mess) const {
    constexpr array<char, 64> k = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '=', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
        'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    array<char, 64> res = {
        'j', 'B', 'q', 't', 'C', 'N', 'a', 'V', 'w', 'F', 'S', '6', '0',
        'p', '3', 'x', 'K', 'i', 'I', 'H', '8', 'G', 'D', '2', 'Y', 'O',
        'R', 'W', '9', 'o', '1', 'k', 's', 'Z', 'l', 'v', '4', 'P', 'X',
        '/', 'U', 'M', 'n', 'g', '=', 'N', 'r', 'J', '5', 'b', 'y', 'Q',
        'z', 'h', 'T', '7', 'A', 'E', 'u', 'c', 'L', 'e', 'd', 'M'};

    size_t seed = 0;
    auto bg = begin(mess);
    auto ed = end(mess);

    while (bg != ed) {
      const auto c = *(bg++);
      seed += c;

      const size_t a = 1103515245 + c;
      const size_t b = 12345 + c;

      for (size_t i = 0; i < res.len(); ++i) {
        res.modify(k.at((seed = (prand(a, b, seed) + res.at(i))) % k.len()), i);
      }
    }

    return res;
  }

  constexpr auto digest(iterator auto b, iterator auto e) {
    return digest(string_view(b, e));
  }
};
}  // namespace vault::crypto

#endif
