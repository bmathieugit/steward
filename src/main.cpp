#define NDEBUG

#include <stew.hpp>

using namespace stew;



string<char> encode_hex(const string<char>& in)
{
  constexpr string_view<char> encode_table = "0123456789abcdef";
  string<char> out(in.size()*2+10);

  for (char c : in)
  {
    out.push(encode_table[c >> 4]);
    out.push(encode_table[c & 15]);
  }

  return out;
}

constexpr char reducehex(char c)
{
  return (c - '0') <= ('9' - '0') ? c - '0' : c - 'a' + 10;
}

char fromhex(char c0, char c1)
{
  return reducehex(c0) << 4 | reducehex(c1);
}

string<char> decode_hex(const string<char>& in)
{
  assert(in.size() % 2 == 0);
  string<char> out(in.size()/2+10);

  auto b = in.begin();
  auto e = in.end();

  while (b != e)
  { 
    char c0 = *b++;
    char c1 = *b++;
    out.push(fromhex(c0, c1));
  }

  return out;
}


int main(int argc, char** argv)
{
  console<char>::printfln("encode hex \0", encode_hex("Hello World"_sv));
  console<char>::printfln("decode hex \0", decode_hex(encode_hex("Hello World"_sv)));
  
  return 0;
}


