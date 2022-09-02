#include <logger.hpp>
#include <csv.hpp>
#include <vault.hpp>
#include <ui.hpp>

#include <string>
#include <vector>


int main(int argc, char **argv)
{
  stew::ui::text_input ti_login("login");
  stew::ui::text_input ti_passw("password");

  std::string login;
  std::string password;

  ti_login.render();
  ti_login.capture(login);
  ti_passw.render();
  ti_passw.capture(password);
  
  std::cout << "login : " << login << "; password : " << password << "\n";

  stew::ui::table table({"login", "password"});
  
  table.push_row({"bmathieugit", "PassWORD!!!"});
  table.push_row({"bmathieugit52", "AzertyUi"});
  table.push_row({"bmathieugit_alter", "PassWORD!!!-6q5s4d65q4sd54M*qsdd"});

  table.render();

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
