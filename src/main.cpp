#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>

#include <string>
#include <vector>

#include <unistd.h>

//  vault read github.com/bmathieugit
//  1     2    3
int main(int argc, char **argv)
try {
  if (argc > 2)
  {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.size() >= 2)
    {
      if (args[0] == "vault")
      {
        stew::vault_agent<stew::cesar_crypto_algorithm> vaulta;
        vaulta.process(args);
      }
    }
  }

  return EXIT_SUCCESS;
} catch (std::exception& ex)
{
  std::cout << ex.what() << std::endl;
}
