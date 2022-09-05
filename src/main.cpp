#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>
#include <bit>

#include <array>
#include <iostream>

int main(int argc, char **argv)
{
  /*
  std::string s;
  std::cout << "login : "<<'\n';
  std::cout << "mdp   : "<<'\n';
  std::cout << "\033[2A";
  std::cout << "\033[8C";
  std::cin >> s;
  std::cout << "\033[8C";
  std::cin >> s;
 */
  std::array<std::array<char, 80>, 60> buffer;

  for (std::size_t i(0); i < buffer.size(); ++i)
    for (std::size_t j(0); j < buffer[i].size(); ++j)
      buffer[i][j] = ' ';

  for (std::size_t k(0); k < 10; ++k)
  {
    std::string m;
    buffer[59][79] = '0' + k;
    for (std::size_t i(0); i < buffer.size(); ++i)
      for (std::size_t j(0); j < buffer[i].size(); ++j)
        std::cout << buffer[i][j];
    
    std::cin >> m;
    std::cout << "\033[2J";
    std::cout << "\033[H";
  }
  /* if (argc > 2)
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
 } */

  return EXIT_SUCCESS;
}
