#include <args/parse.hpp>

int main() {
  map<string, string> keepass;
  auto key = istream("password");
  vault::crypto::ares ares(key);

  fnv_ostream<64> fnv;
  sout << "mdp ajouté "
       << (keepass.add(ares.crypt(cstring("x87xk7KS")), istream("BM383A1N")))
       << '\n';
  sout << "length " << keepass.len() << '\n';
  sout << ares.decrypt(keepass.at(istream("BM383A1N"))) << '\n';

  string s0 = istream("BM383A1N"), s1 = istream("BM383A1N");
  sout << equals(s0, s1) << '\n';
}
