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
void printdb(const stew::dbf::storage::mem::node<T> &n)
{
  std::cout << n.name() << '\n';

  for (const auto &[_, i] : n.childs())
  {
    if (stew::dbf::storage::mem::instance_of<stew::dbf::storage::mem::node<T>>(i))
    {
      printdb(std::get<stew::dbf::storage::mem::node<T>>(i));
    }
    else
    {
      std::cout << std::get<stew::dbf::storage::mem::leaf<T>>(i).name()
                << " : " << std::get<stew::dbf::storage::mem::leaf<T>>(i).data() << '\n';
    }
  }
}

int main()
{
  using namespace std::literals::string_literals;

  std::filesystem::path p = "lol:";
  stew::dbf::db::schema_name sc = "lol";

  // std::optional<stew::person> op = stew::person();

  stew::dbf::storage::mem::root<std::string> r = stew::dbf::storage::mem::make_root<std::string>("ldap");
  // std::cout << std::boolalpha << r.insert("coucou"s, "ldap", "toto", "person", "amaidqjsl");
  // std::cout << std::boolalpha << r.insert("hello"s, "ldap", "toto", "person", "amaidqjsl");

  // // maintenant que j 'ai un systeme d'insertion dans un arbre en mémoire, jedois povuoir supprimer un élément.
  // // un premier essais de destruction pour vérifier qu'on ne supprime que ce qui existe
  // std::cout << std::boolalpha << r.remove("ldap", "toto", "person", "amaidqjsl_bad") << '\n';
  // // un second essais qui sera lui une réussite
  // std::cout << std::boolalpha << r.remove("ldap", "toto", "person", "amaidqjsl") << '\n';

  // // maintenant que je peux supprimer et ajouter un élément au sein de la base de donnée, je vais
  // // essayer d'ajouter un élément dans un endroit ou il ne devrait pas être.

  // std::cout << std::boolalpha << r.insert("hello2"s, "ldap", "toto", "person") << '\n';

  // // On va maintenant essayer d'inserer une seconde person dans le meme espace
  // std::cout << std::boolalpha << r.insert("hello2"s, "ldap", "toto", "person", "aziodj") << '\n';

  // // ici la lib storage::mem est donc opérationnelle.
  // // Maintenant on va créer une API permettant de s'abstraire du type de storage.

  stew::dbf::storage::api::memory<std::string> mem("ldap");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name2");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name3");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name4");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name5");

  // maintenant que j'ai une api minimale pour la couche storage, je peux passer à la mise en place
  // de la couche supérieure. Il va s'agir d'une api de requetage.
  // on y verra les fonctions suivante:  exists, select, insert, update, remove, count

  std::optional<std::string> res = stew::dbf::query::select(mem, "people", "name");
  bool res2 = stew::dbf::query::insert(mem, "martin"s, "people", "name2");
  bool res3 = stew::dbf::query::update(mem, "martin"s, "people", "name2");
  bool res4 = stew::dbf::query::remove(mem, "people", "name2");
  std::size_t cnt = stew::dbf::query::count(mem, "people", "name2");
  bool ex = stew::dbf::query::exists(mem, "people", "name2");

  return 0;
}