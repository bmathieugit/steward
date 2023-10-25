#ifndef __stew_vault_crypto_ares_hpp__
#define __stew_vault_crypto_ares_hpp__

#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>
#include <core/string.hpp>
#include <vault/crypto/mash.hpp>

namespace vault::crypto {

class ares {
 public:
  constexpr fixed_string crypt(const char_collection auto& key,
                               const char_collection auto& mess) const {
    array<char, 64> keycp = mash{}.digest(istream(key));

    // array<array<char, 64>, 64> matrix;

    // for (size_t i = 0; i < 64; ++i) {
    //   matrix.add(array<char, 64>());
    //   size_t seed = keycp.at(i);

    //   for (size_t j = 0; j < 64; ++j) {
    //     matrix.at(i).add((seed = rand(seed)) % 64);
    //   }
    // }

    fixed_string crypted(mess);

    for (size_t occ = 0; occ < key.len(); ++occ) {
      for (size_t i = 0; i < crypted.len(); ++i) {
        auto c = crypted.at(i);
        auto ckey = keycp.at(i % keycp.len());
//        auto cm = matrix.at(i % keycp.len()).at(occ);
        auto cc = (c ^ ckey) + ckey /*+ cm*/;
        crypted.modify(i, cc);
      }
    }

    return crypted;
  }

  constexpr fixed_string decrypt(const char_collection auto& key,
                                 const char_collection auto& mess) const {
    array<char, 64> keycp = mash{}.digest(istream(key));

    // array<array<char, 64>, 64> matrix;

    // for (size_t i = 0; i < 64; ++i) {
    //   matrix.add(array<char, 64>());
    //   size_t seed = keycp.at(i);

    //   for (size_t j = 0; j < 64; ++j) {
    //     matrix.at(i).add((seed = rand(seed)) % 64);
    //   }
    // }

    fixed_string crypted(mess);

    for (size_t occ = 0; occ < key.len(); ++occ) {
      for (size_t i = 0; i < crypted.len(); ++i) {
        auto c = crypted.at(i);
        auto ckey = keycp.at(i % keycp.len());
        // auto cm = matrix.at(i % keycp.len()).at(occ);
        auto cc = (c - /*cm -*/ ckey) ^ ckey;
        crypted.modify(i, cc);
      }
    }

    return crypted;
  }
};

}  // namespace vault::crypto

#endif