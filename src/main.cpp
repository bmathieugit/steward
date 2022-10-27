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



int main()
{
  using namespace std::literals::string_literals;

  std::filesystem::path p = "lol:";
  stew::dbf::db::schema_name sc = "lol";

  // std::optional<stew::person> op = stew::person();

  stew::dbf::storage::mem::child<int> r = stew::dbf::storage::mem::make_root<int>("ldap");
  stew::dbf::storage::mem::append(r, "ldap/person/amdkqmzdk", 12);

  stew::dbf::db::create_schema(stew::dbf::db::schema_name("ldap"));
  stew::dbf::db::connection conn = stew::dbf::db::connect(std::string("ldap"));

  if (conn.opened())
  {
    stew::dbf::db::create_table<stew::person>(conn);
  }
  // la table ext créée

  return 0;
}