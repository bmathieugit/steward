#include <dbfile.hpp>

#include <string>
#include <tuple>

#include <filesystem>
#include <iostream>

void printdb(const stew::dbf::storage::mem::root &r)
{
  std::cout << r.name() << '\n';

  for (const auto &[_, i] : r.childs())
  {
    if (i.index() == 0)
    {
      printdb(std::get<stew::dbf::storage::mem::root>(i));
    }
    else
    {
      std::cout << std::get<stew::dbf::storage::mem::leaf>(i).name()
                << " : " << std::any_cast<std::string>(std::get<stew::dbf::storage::mem::leaf>(i).data()) << '\n';
    }
  }
}

int main()
{
  using namespace std::literals::string_literals;

  stew::dbf::storage::mem::root r("ldap");

  stew::dbf::storage::api::memory mem("ldap");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name2");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name3");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name4");
  std::cout << std::boolalpha << mem.insert("martin"s, "people", "name5");

  // std::optional<std::string> res = stew::dbf::query::select(mem, "people", "name");
  // bool res2 = stew::dbf::query::insert(mem, "martin"s, "people", "name2");
  // bool res3 = stew::dbf::query::update(mem, "martin"s, "people", "name2");
  // bool res4 = stew::dbf::query::remove(mem, "people", "name2");
  // std::size_t cnt = stew::dbf::query::count(mem, "people", "name2");
  // bool ex = stew::dbf::query::exists(mem, "people", "name2");

  return 0;
}