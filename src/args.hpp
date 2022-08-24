#ifndef __steward_args_hpp__
#define __steward_args_hpp__

#include <string>
#include <vector>
#include <string_view>
#include <algorithm>

/**
 * @brief Le but de cette librairie est de permettre le handling des
 * arguments passer dans la ligne de commande.
 * Il faut donc que l'on puisse faire de la detection de présence
 * d'un argument, l'extraction de sa valeur et la transformation
 * de cette valeur dans un type souhaité natif (string, int, float).
 *
 * Dans un second temps on doit pouvoir permettre la transformation
 * d'un type dans un objet plus avancer (eg: email, date, timestamp...)
 *
 * Nous avons donc besoins d'un objet qui va être le support des arguments
 * présents dans la ligne de commande.
 * De cette objet nous aurons une fonction de recherche (contains) qui
 * retourne un booleen true si l'argument est présent et false sinon.
 * Si l'argument est présent alors nous pouvons savoir s'il sagit d'un flag
 * ou bien d'un argument valorisé (bool valued)
 */
namespace stew
{
  class args_handler
  {
    std::vector<std::string> args;

  public:
    explicit args_handler(char **argv, int argc)
        : args(argc)
    {
      for (int i = 0; i < argc; ++i)
        args.push_back(std::string(argv[i]));
    }

    args_handler() = delete;
    args_handler(const args_handler &) = default;
    args_handler(args_handler &&) = default;
    args_handler &operator=(const args_handler &) = default;
    args_handler &operator=(args_handler &&) = default;
    ~args_handler() = default;

  public:
    bool contains(std::string_view arg) const
    {
      return std::ranges::find_if(args,
                                  [&arg](const std::string &a)
                                  {
                                    return a.starts_with(arg);
                                  }) != args.end();
    }

    bool valued(std::string_view arg) const
    {
      auto found = std::ranges::find_if(args, [&arg](const std::string &a)
                                        { return a.starts_with(arg); });

      if (found != args.end())
      {
        std::size_t i = (*found).find('=');
        return i != std::string::npos && i < (*found).size();
      }
      else
        return false;
    }
  };
}

#endif