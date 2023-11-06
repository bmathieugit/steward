#ifndef __stew_crypto_sha_hpp__
#define __stew_crypto_sha_hpp__

#include <core/array.hpp>
#include <core/core.hpp>

namespace vault::crypto {

class mash {
 public:
  constexpr auto digest(char_input_stream auto mess) const {
    constexpr array k({'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
                       'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                       'w', 'x', 'y', 'z', '=', '/', '0', '1', '2', '3', '4',
                       '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
                       'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                       'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'});

    array res({'j', 'B', 'q', 't', 'C', 'N', 'a', 'V', 'w', 'F', 'S', '6', '0',
               'p', '3', 'x', 'K', 'i', 'I', 'H', '8', 'G', 'D', '2', 'Y', 'O',
               'R', 'W', '9', 'o', '1', 'k', 's', 'Z', 'l', 'v', '4', 'P', 'X',
               '/', 'U', 'M', 'n', 'g', '=', 'N', 'r', 'J', '5', 'b', 'y', 'Q',
               'z', 'h', 'T', '7', 'A', 'E', 'u', 'c', 'L', 'e', 'd', 'M'});

    size_t seed = 0;

    while (mess.has()) {
      auto c = mess.next();
      seed += c;
      size_t a = 100000000 + c;
      size_t b = 12345 + c;

      for (size_t i = 0; i < res.len(); ++i) {
        auto nc = k.at((seed = (prand(a, b, seed) + res.at(i))) % k.len());
        res.modify(i, nc);
      }
    }

    return res;
  }
};
}  // namespace vault::crypto

#endif
