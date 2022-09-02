#ifndef __stew_vault_hpp__
#define __stew_vault_hpp__

#include <csv.hpp>
#include <logger.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>
#include <fstream>
#include <iostream>

namespace stew
{
  struct vault_entry
  {
    std::string key;
    std::string secret;
  };

  template <>
  struct csv::csv_entry<vault_entry>
  {
    void from(const vault_entry &v, auto op) const
    {
      return op(v.key, v.secret);
    }

    void to(vault_entry &v, auto op) const
    {
      return op(v.key, v.secret);
    }
  };

  template <typename CA>
  concept crypto_algorithm = requires
  {
    {
      CA::encrypt(vault_entry{})
      } -> std::same_as<vault_entry>;
    {
      CA::decrypt(vault_entry{})
      } -> std::same_as<vault_entry>;
  };

  struct identity_crypto_algorithm
  {
    static vault_entry encrypt(const vault_entry &ve)
    {
      return vault_entry{ve.key, ve.secret};
    }

    static vault_entry decrypt(const vault_entry &ve)
    {
      return vault_entry{ve.key, ve.secret};
    }
  };

  struct cesar_crypto_algorithm
  {
    static vault_entry encrypt(const vault_entry &ve)
    {
      std::string encrypted = ve.secret;
      for (char &c : encrypted)
        c = c + 1;
      return vault_entry{ve.key, encrypted};
    }

    static vault_entry decrypt(const vault_entry &ve)
    {
      std::string decrypted = ve.secret;
      for (char &c : decrypted)
        c = c - 1;
      return vault_entry{ve.key, decrypted};
    }
  };

  template <crypto_algorithm CA>
  class vault_service
  {
    std::vector<vault_entry> repo;
    std::string fpath;

  public:
    void load(std::string_view path)
    {
      if (!path.empty())
      {
        fpath = std::string(path);
        std::ifstream ifs(fpath);
        std::string line;
        csv::csv_marshaller csvm;
        if (ifs.is_open())
          while (std::getline(ifs, line))
            repo.push_back(csvm.unmarshall<vault_entry>(line));
        ifs.close();
      }
    }

    void flush()
    {
      if (!fpath.empty())
      {
        std::ofstream ofs(fpath);
        csv::csv_marshaller csvm;

        if (ofs.is_open())
          for (auto &&ve : repo)
            ofs << csvm.marshall(ve) << "\n";

        repo.clear();
        ofs.close();
      }
    }

    void save(const vault_entry &v)
    {
      auto found = find(v.key);
      auto vencrypted = CA::encrypt(v);

      if (found == repo.end())
        repo.push_back(std::move(vencrypted));
      else
        *found = std::move(vencrypted);
    }

    std::optional<vault_entry> read(
        std::string_view key)
    {
      auto found = find(key);

      if (found == repo.end())
        return std::nullopt;

      return std::optional<vault_entry>(CA::decrypt(*found));
    }

    void remove(std::string_view key)
    {
      auto found = find(key);

      if (found != repo.end())
        repo.erase(found);
    }

    bool contains(std::string_view key)
    {
      return find(key) != repo.end();
    }

    void foreach (auto cb)
    {
      for (const vault_entry &ve : repo)
        cb(CA::decrypt(ve));
    }

  private:
    std::vector<vault_entry>::iterator find(std::string_view key)
    {
      return std::find_if(
          repo.begin(), repo.end(),
          [key](const vault_entry &v)
          { return v.key == key; });
    }
  };

  template <crypto_algorithm CA>
  class vault_agent
  {
    vault_service<CA> vaults;

  public:
    void process(const std::vector<std::string> &args)
    {
      if (args.size() >= 4 && args[1] == "read")
        read(args[2], args[3]);
      else if (args.size() >= 5 && args[1] == "save")
        save(args[2], args[3], args[4]);
      else if (args.size() >= 4 && args[1] == "remove" or args[1] == "rm")
        remove(args[2], args[3]);
      else if (args.size() >= 2 && args[1] == "list")
        list(args[2]);
    }

  private:
    void read(const std::string &key, const std::string &path)
    {
      vaults.load(path);
      logger::flog(std::cout, vaults.read(key).value_or(stew::vault_entry()).secret);
    }

    void save(const std::string &key, const std::string &secret, const std::string &path)
    {
      vaults.load(path);
      vaults.save(vault_entry{key, secret});
      vaults.flush();
    }

    void remove(const std::string &key, const std::string &path)
    {
      vaults.load(path);
      vaults.remove(key);
      vaults.flush();
    }

    void list(const std::string &path)
    {
      vaults.load(path);
      vaults.foreach ([](const vault_entry &ve)
                      { logger::flog(std::cout, '"', ve.key, "\":", ve.secret); });
    }
  };
}

#endif