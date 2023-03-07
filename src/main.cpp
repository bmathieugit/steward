#define NDEBUG

#include <stew.hpp>

using namespace stew;
/*
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

struct password {
  string<char> _key;
  string<char> _login;
  string<char> _passwd;
};

constexpr bool operator<(const password& p0, const password& p1) {
  return p0._key < p1._key;
}

constexpr bool operator!=(const password& p0, const password& p1) {
  return p0._key != p1._key;
}

inline int strlen(const char* s)
{
  auto cur = s;
  while (*cur != '\0')
    ++cur;
  return cur -s;
}

class args {
 public:
  fixed_vector<string_view<char>> _argv;

 public:
  args(int argc, char** argv) : _argv(argc) {
    for (int i = 0; i < argc; ++i) {
      _argv.push(string_view<char>(argv[i], argv[i]+strlen(argv[i])));
    }
  }

  bool contains(string_view<char> shortkey, string_view<char> key) {
    return find(_argv, val(key) == p0 || val(shortkey) == p0);
  }

  string_view<char> get(string_view<char> shortkey, string_view<char> key) {
    auto fnd = find(
        _argv, [&](string_view<char> c) { return c == shortkey || c == key; });

    if (fnd != _argv.end()) {
      ++fnd;

      if (fnd != _argv.end()) return *fnd;
    }

    return ""_sv;
  }
};
*/

int main(int argc, char** argv) {
  /*  args a(argc, argv);

    for (string_view<char> arg : a._argv) {
      console<char>::printfln("arg in argv : '\0'", arg);
    }

    if (a.contains("-k", "--key")) {
      string_view<char> key = a.get("-k", "--key");

      set<password> ss;

      ss.push(password{"github.com"_sv, "BobSmith2023"_sv,
    "bobsimbel12!!!"_sv}); ss.push(password{"microsoft.com"_sv,
    "ArandJuvini"_sv, "MeliSanDre@/!!%"_sv});

      auto fnd = find(ss, [&key](const password& p) {
        console<char>::printfln("\0(\0) vs \0(\0)", p._key, p._key.size(), key,
                                key.size());
        return p._key == key;
      });

      if (fnd != ss.end()) {
        console<char>::printfln("'\0'", (*fnd)._passwd);
      }
    } else {
      console<char>::println("key not found");
    }*/

  
  string_view<char> s = str::fixed::from("dot");
  auto s0 = str::fixed::from("coucot");
  auto s1 = str::fixed::from("coucou");
   
  console<char>::printfln("\0", str::cmp<char>(s0, s1));

  return 0;
}
