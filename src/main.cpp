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
  auto from(const person& p)
  {
    return stew::csv::make_csv_output(p.name, p.firstname);
  }

  auto to (person& p)
  {
    return stew::csv::make_csv_input(p.name, p.firstname);
  }
};


auto to_csv_input(person & p)
{
  return stew::csv::make_csv_input(p.name, p.firstname);
}

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
