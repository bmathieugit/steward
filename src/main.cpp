#define NDEBUG

#include <stew.hpp>

using namespace stew;

string<char> encode_hex(const string<char>& in) {
  constexpr string_view<char> encode_table = "0123456789abcdef";
  string<char> out(in.size() * 2);

  for (char c : in) {
    out.push(encode_table[c >> 4]);
    out.push(encode_table[c & 15]);
  }

  return out;
}

constexpr char reducehex(char c) {
  return (c - '0') <= ('9' - '0') ? c - '0' : c - 'a' + 10;
}

constexpr char fromhex(char c0, char c1) {
  return reducehex(c0) << 4 | reducehex(c1);
}

string<char> decode_hex(const string<char>& in) {
  assert(in.size() % 2 == 0);

  string<char> out(in.size() / 2);

  auto b = in.begin();
  auto e = in.end();

  while (b != e) {
    char c0 = *b++;
    char c1 = *b++;
    out.push(fromhex(c0, c1));
  }

  return out;
}

// J'ai maintenant une fonction de transformation de mot de passe chiffré en
// quelque chose de lisible par un humain. On va donc faire un algorithme de
// chiffrement basique.

string<char> encrypt(const string<char>& in, const string<char>& master) {
  string<char> out;

  for (char c : in) {
    for (char cm : master) {
      c += cm;
    }

    out.push(c);
  }

  return out;
}

string<char> decrypt(const string<char>& in, const string<char>& master) {
  string<char> out;

  for (char c : in) {
    for (char cm : master) {
      c -= cm;
    }

    out.push(c);
  }

  return out;
}

// J'ai donc deux fonctions capables de chiffrer et déchiffrer n'importe quoi
// je veux maintenant enregistrer ce résultat dans une map ou le mot de passe
// sera stocker avec un nom (clé)

struct password {
  string<char> _key;
  string<char> _passwd;
};

constexpr bool operator<(const password& p0, const password& p1) {
  return p0._key < p1._key;
}

int main(int argc, char** argv) {

  set<string<char>> ss;
  ss.push("coucou"_sv);
  ss.push("tutu"_sv);
  ss.push("tutu"_sv);
  ss.push("coucouc2"_sv);
  ss.push("coucou"_sv);

  for (auto && s : ss)
  {
    console<char>::printfln("\0", s);
  }


  return 0; }
