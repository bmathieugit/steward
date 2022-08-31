#ifndef __stew_vault_hpp__
#define __stew_vault_hpp__

#include <csv.hpp>
//#include <repository.hpp>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <algorithm>
#include <fstream>

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
    std::size_t load(std::string_view path)
    {
      fpath = std::string(path);
      std::ifstream ifs(fpath);
      std::string line;
      csv::csv_marshaller csvm;

      if (ifs.is_open())
        while (std::getline(ifs, line))
          repo.push_back(csvm.unmarshall<vault_entry>(line));

      return repo.size();
    }

    std::size_t flush()
    {
      std::size_t s = repo.size();
      std::ofstream ofs(fpath + "2");
      csv::csv_marshaller csvm;

      if (ofs.is_open())
        for (auto &&ve : repo)
          ofs << csvm.marshall(ve) << "\n";

      repo.clear();
      return s;
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

  struct vault_agent
  {
    void process(const std::vector<std::string> &args)
    {
      if (!args.empty())
      {
        if (args[0] == "read")
        {
          read(args[1]);
        }
        else if (args[0] == "add")
        {
          add(args[1], args[2]);
        }
        else if (args[0] == "update")
        {
          update(args[1], args[2]);
        }
        else if (args[0] == "remove")
        {
          remove(args[1]);
        }
        else if (args[0] == "list")
        {
          list();
        }
      }
    }

  private:
    void read(const std::string &key)
    {
    }

    void add(const std::string &key, const std::string &secret)
    {
    }

    void update(const std::string &key, const std::string &secret)
    {
    }

    void remove(const std::string &key)
    {
    }

    void list()
    {
    }
  };
}

#endif