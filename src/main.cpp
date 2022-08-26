#include <logger.hpp>
#include <csv.hpp>
#include <string>

struct person
{
  std::string name;
  std::string firstname;
};

template<>
struct stew::csv::csv_entry<person>
{
  void from(const person& p, std::stringstream& ss, const auto& marsh)
  {
    marsh.marshall_entry(ss, p.name, p.firstname);
  }

  void to (person& p, std::string_view s, const auto& marsh)
  {
    marsh.unmarshall_entry(s, p.name, p.firstname);
  }
};


int main(int argc, char** argv)
{
  
  stew::logger::stew("Hello Sir !");
  stew::logger::stew("What can I do for you ?");

  person p{"MATHIEU", "Benajmin"};

  stew::csv::csv_marshaller csv;

  std::string line = csv.marshall(p);
  person p2 = csv.unmarshall<person>(line);

  std::cout << line << '\n';
  std::cout << p2.firstname << p2.name << '\n';

  return EXIT_SUCCESS;
}
