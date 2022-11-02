#include <dbfile.hpp>

#include <string>
#include <tuple>

#include <filesystem>
#include <iostream>

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

struct address
{
  std::string street;
  int num;
};

struct person
{
  std::string name;
  std::string firstname;
  std::vector<address> addrs;
};

namespace stew::dbf::storage::format
{

  void unmarshall(const tree &t, address &a, std::string_view parent_path)
  {
    a.num = (int)t.get<number>(std::string(parent_path) + "/num").value();
    a.street = t.get<string>(std::string(parent_path) + "/street").value();
  }

  void unmarshall(const tree &t, person &p, std::string_view parent_path)
  {
    unmarshall(t, p.addrs, std::string(parent_path) + "/address");
    p.name = t.get<string>(std::string(parent_path) + "/name").value();
    p.firstname = t.get<string>(std::string(parent_path) + "/firstname").value();
  }

  void marshall(tree &t, const address &a, std::string_view parent_path)
  {
    t.set(std::string(parent_path) + "/street", a.street);
    t.set(std::string(parent_path) + "/num", (number)a.num);
  }

  void marshall(tree &t, const person &p, std::string_view parent_path)
  {
    t.set(std::string(parent_path) + "/name", p.name);
    t.set(std::string(parent_path) + "/firstname", p.firstname);

    marshall(t, p.addrs, std::string(parent_path) + "/address");

    t.size_of( std::string(parent_path) + "/address", p.addrs.size());
  }
}

int main()
{
  using namespace std::literals::string_literals;

  stew::dbf::storage::mem::root<std::string> r = stew::dbf::storage::mem::make_root<std::string>("ldap");

  stew::dbf::storage::api::memory<std::string> mem("ldap");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name2");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name3");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name4");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name5");

  std::optional<std::string> res = stew::dbf::query::select(mem, "people", "name");
  bool res2 = stew::dbf::query::insert(mem, "martin"s, "people", "name2");
  bool res3 = stew::dbf::query::update(mem, "martin"s, "people", "name2");
  bool res4 = stew::dbf::query::remove(mem, "people", "name2");
  std::size_t cnt = stew::dbf::query::count(mem, "people", "name2");
  bool ex = stew::dbf::query::exists(mem, "people", "name2");

  stew::dbf::storage::format::tree tr;

  person p{"smith", "john", {{"cross road stree", 12}}};

  stew::dbf::storage::format::tree pt = stew::dbf::storage::format::marshall(p);
  person p2 = stew::dbf::storage::format::unmarshall<person>(pt);
  std::cout << p2.name << p2.firstname << p2.addrs[0].num << p2.addrs[0].street;

  return 0;
}