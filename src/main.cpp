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
  stew::ui::screen<80, 60> scr;
  scr.begin_write();
  scr.write("coucou %\n");
  scr.write("comment allez vous monsieur ? (bien, bof) %\n");
  scr.end_write();
  scr.at_origin();
  scr.at_next_marker();
  scr.display();

  std::string name = scr.capture();
  scr.at_next_marker();
  scr.display();
  std::string rep = scr.capture();

  std::cout << "vous vous appelez " << name << " et vous allez " << rep << '\n';

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
