#include <logger.hpp>
#include <csv.hpp>
#include <string>
#include <args.hpp>

struct person
{
  std::string name;
  std::string firstname;
};

auto to_csv_output(const person &p)
{
  return stew::csv::make_csv_output(p.name, p.firstname);
}

auto to_csv_input(person & p)
{
  return stew::csv::make_csv_input(p.name, p.firstname);
}

int main(int argc, char** argv)
{
  
  stew::logger::stew("Hello Sir !");
  stew::logger::stew("What can I do for you ?");

  person p{"MATHIEU", "Benajmin"};

  std::string line = stew::csv::to_csv(p, ';', '"', '"');
  person p2 = stew::csv::from_csv<person>((std::string_view) line, ';', '"', '"');

  std::cout << line << '\n';
  std::cout << p2.firstname << p2.name << '\n';

  stew::args_handler argsh = stew::args_handler(argv, argc);
  
  stew::logger::stew("Found or not : ", argsh.contains("coucou"));

  return EXIT_SUCCESS;
}
