#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>

#include <string>
#include <vector>

//  vault read github.com/bmathieugit
//  1     2    3
int main(int argc, char **argv)
{
  stew::logger::stew("Hello Sir !");
  stew::logger::stew("What can I do for you ?");

  if (argc > 2)
  {
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.size() == 4)
    {
      if (args[0] == "vault")
      {
        stew::vault_agent vaulta;
        vaulta.process(args);
      }
    }
  }

  return EXIT_SUCCESS;
}
