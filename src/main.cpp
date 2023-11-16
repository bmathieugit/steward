#include <core/chars.hpp>
#include <core/file.hpp>
#include <core/string.hpp>
#include <core/map.hpp>
#include <args/argsparse.hpp>


int main(int argc, char** argv) {

  map<string, int> m;
  m.add(12, iter("Bob"));
  m.add(15, iter("Leon"));
  write(sout, "note de Bob ", m.at(iter("Bob")), '\n');


  // args::program p(iter("test"));
  // p.add_option(iter("--valid"), 'v', true, iter("true"),
  //                     iter("est ce que c'est valide ou pas"));
  // p.add_option(iter("--file"), 'f', true, iter(""),
  //                     iter("fichier dont le nom nous est très utile"));
                                           
  // p.parse(argc, argv);
  
  // write(sout, p.help());

  // write(sout, "value detected : ", p.get_value(iter("--valid")), '\n');
  return 0;
}
