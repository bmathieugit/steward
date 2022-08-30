#ifndef __stew_vault_hpp__
#define __stew_vault_hpp__

#include <csv.hpp>
#include <repository.hpp>
#include <string>
#include <string_view>
#include <chrono>

namespace stew
{
  using timestamp = std::chrono::time_point<std::chrono::system_clock>;
  /**
   * @brief La lib vault va permettre de mettre en place un moyen de stocker et lire
   * des entrés secret dans un storage type csv.
   * Il faut donc d'abord mettre en place la structure de données qui va permettre de
   * porter les informations d'un secret
   */
  struct vault_entry
  {

    std::string key;
    std::string secret;
    timestamp create_tp;
    timestamp update_tp;
    bool saved = false;
  };

  /**
   * @brief J'ai maintenant une structure de base. il faut donc un mecanisme pour
   * enregistrer cette structure dans un csv. Il faut donc que je mettre en place
   * une implementattion de csv_entry pour cette structure.
   */
  template <>
  struct csv::csv_entry<vault_entry>
  {
    void from(const vault_entry &v, auto op) const
    {
      return op(v.key, v.secret, v.create_tp, v.update_tp);
    }

    void to(vault_entry &v, auto op) const
    {
      return op(v.key, v.secret, v.create_tp, v.update_tp);
    }
  };

  /**
   * @brief Je peux maintenant sérialiser ma structure sous la forme d'un csv.
   * Du moins en théorie. Il va falloir voir pour convertir les time_point en
   * string via la surcharge de l'operateur << de std::ostream.
   * Mais pour l'instant passons plutôt au service qui effectuer ces sérialisations
   * et deserialisation. Il va également proposer un moyen de retourner le secret en
   * fonction de la clé qu'on lui aura proposer. Dans un premier temps les secrets
   * ne seront pas chiffrés. Mais je vais quand meme mettre la possibilité de le
   * faire dans l'entête de la classe.
   */

  class vault_service
  {
    using vault_repository = repository<std::string, vault_entry>;
    vault_repository repo;

  public:
    void save(const vault_entry &v)
    {
      if (!v.saved)
        v.create_tp = std::chrono::system_clock::now();
      v.update_tp = std::chrono::system_clock::now();
      repo.push(v.key, v);
    }

    vault_entry read(std::string_view key)
    {
      return repo.get(std::string(key));
    }

    void remove(std::string_view key)
    {
      repo.erase(std::string(key));
    }
  };
}

#endif