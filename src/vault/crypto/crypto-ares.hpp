#ifndef __stew_vault_crypto_ares_hpp__
#define __stew_vault_crypto_ares_hpp__

#include <core/char-istream.hpp>
#include <core/char-ostream.hpp>

namespace vault::crypto {
class ares {
 public:
  constexpr void crypt(char_input_stream auto key,
                       char_input_stream auto is,
                       char_output_stream auto os);
                       
  constexpr void decrypt(char_input_stream auto key,
                         char_input_stream auto is,
                         char_output_stream auto os);
};
}  // namespace vault::crypto

#endif