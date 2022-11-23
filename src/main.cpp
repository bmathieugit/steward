// #include <dbfile.hpp>

// #include <string>
// #include <vector>
// #include <iostream>
// #include <list>

#include <stew.hpp>
//#include <iostream>

struct person
{
  stew::string name;
  stew::string fname;
};

namespace stew
{
  template <>
  class formatter<person>
  {
  public:
    static void to(string &os, const person &p)
    {
      formatter<string>::to(os, p.name);
      formatter<string>::to(os, p.fname);
    }
  };
}

namespace stew
{
  template <>
  class formatter<int>
  {
  public:
    static void to(string &os, const int &i)
    {
      class StackArray
      {
        char data[40];
        int i = -1;

      public:
        void push(char c) noexcept { data[++i] = c; }
        char pop() noexcept { return data[i--]; }
        bool empty() noexcept { return i == -1; }
      };

      bool neg = i < 0;

      int tmp = neg ? -i : i;

      StackArray tbuff;

      if (tmp == 0)
        formatter<char>::to(os, '0');
      else
        while (tmp != 0)
        {
          tbuff.push("0123456789"[tmp % 10]);
          tmp /= 10;
        }

      if (neg)
        formatter<char>::to(os, '-');

      while (!tbuff.empty())
        formatter<char>::to(os, tbuff.pop());
    }
  };

}

int main()
{
  using namespace stew;

  string s("prout");
  string_view sv("prout");

  person bob{"Dilan", "Bob"};


  cout.write(format(" hello {} tu es le {} eme!\n", bob, 2));
  

  return 0;
}