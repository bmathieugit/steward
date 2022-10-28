#include <dbfile.hpp>

#include <string>
#include <tuple>

#include <filesystem>
#include <iostream>

namespace stew
{
  namespace dbf::db
  {
    using schema_name = std::string;

    void create_schema(const schema_name &sch)
    {
      namespace fs = std::filesystem;

      if (!fs::exists(sch))
      {
        fs::create_directories(fs::path(sch));
      }
    }
    enum class conn_status
    {
      opened,
      closed,
      error
    };

    class connection
    {
      schema_name _schema;
      conn_status _status = conn_status::closed;

    public:
      ~connection() = default;
      connection() = default;
      connection(const connection &) = delete;
      connection(connection &&) = default;
      connection &operator=(const connection &) = delete;
      connection &operator=(connection &&) = default;

    public:
      const schema_name &target() const;
      const bool opened() const;
      const bool closed() const;
      const bool error() const;
      const conn_status status() const;

    public:
      friend connection connect(const schema_name &sch);
      friend connection connect(schema_name &&sch);
    };

    const schema_name &connection::target() const
    {
      return _schema;
    }

    const bool connection::opened() const
    {
      return _status == conn_status::opened;
    }

    const bool connection::closed() const
    {
      return _status == conn_status::closed;
    }

    const bool connection::error() const
    {
      return _status == conn_status::error;
    }

    const conn_status connection::status() const
    {
      return _status;
    }

    connection connect(const schema_name &sch)
    {
      connection conn;
      conn._schema = sch;
      return {};
    }

    connection connect(schema_name &&sch)
    {
      namespace fs = std::filesystem;

      connection conn;
      conn._schema = std::move(sch);

      if (fs::exists(sch))
      {
        conn._status = conn_status::opened;
      }
      else
      {
        conn._status = conn_status::error;
      }

      return conn;
    }

    template <typename T>
    struct table_descriptor;

    template <typename T>
    struct column_descriptor
    {
      std::string name;
    };

    template <typename T>
    void create_table(connection &conn)
    {
      namespace fs = std::filesystem;
      namespace io = std;

      table_descriptor<T> td;

      if (conn.opened())
      {
        fs::path t(conn.target());
        t = t / td.name;

        if (!fs::exists(t))
        {
          fs::create_directory(t);
        }

        std::apply([](auto &&...cd)
                   { ((io::cout << cd.name), ...); },
                   td.describe());
      }
    }

  }
}

namespace stew
{
  struct person
  {
    std::string name;
    std::string fname;
  };

  namespace dbf::db
  {
    template <>
    struct table_descriptor<stew::person>
    {
      std::string name = "person";

      std::tuple<column_descriptor<std::string>,
                 column_descriptor<std::string>>
      describe()
      {
        return {{"name"}, {"fname"}};
      }
    };
  }
}

template <typename T>
void printdb(const stew::dbf::storage::mem::child<T> &c)
{
  if (stew::dbf::storage::mem::instance_of<stew::dbf::storage::mem::node<T>>(c))
  {
    std::cout << std::get<stew::dbf::storage::mem::node<T>>(c)._name << '\n';

    for (const auto &[_, i] : std::get<stew::dbf::storage::mem::node<T>>(c)._childs)
    {
      printdb(i);
    }
  }
  else
  {
    std::cout << std::get<stew::dbf::storage::mem::leaf<T>>(c)._name
              << " : " << std::get<stew::dbf::storage::mem::leaf<T>>(c)._data;
  }
}

int main()
{
  using namespace std::literals::string_literals;

  std::filesystem::path p = "lol:";
  stew::dbf::db::schema_name sc = "lol";

  // std::optional<stew::person> op = stew::person();

  stew::dbf::storage::mem::child<std::string> r = stew::dbf::storage::mem::make_root<std::string>("ldap");
  stew::dbf::storage::mem::insert(r, "coucou"s, "ldap", "toto", "person", "amaidqjsl");
  stew::dbf::storage::mem::insert(r, "hello"s, "ldap", "toto", "person", "amaidqjsl");

  printdb(r);
  // maintenant que j 'ai un systeme d'insertion dans un arbre en mémoire, jedois povuoir supprimer un élément. 

  // un premier essais de destruction pour vérifier qu'on ne supprime que ce qui existe
  std::cout << std::boolalpha <<  stew::dbf::storage::mem::remove(r, "ldap", "toto", "person", "amaidqjsl_bad") << '\n';

  // un second essais qui sera lui une réussite
  std::cout << std::boolalpha << stew::dbf::storage::mem::remove(r, "ldap", "toto", "person", "amaidqjsl") << '\n';

  printdb(r);

  std::cout << std::boolalpha << stew::dbf::storage::mem::remove(r, "ldap") << '\n';

  printdb(r);

  return 0;
}