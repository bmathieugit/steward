// #include <dbfile.hpp>

#include <stew.hpp>

using namespace stew;

template <typename T>
class formatter2;

template <integral T>
class formatter2<T>
{
public:
  static void from(string_view is, T &t)
  {
    auto b = is.begin();
    auto f = find(is, [](char c)
                  { return !('0' <= c && c <= '9'); });

    view n(b, f);
    t = 0;

    if (!n.empty())
    {
      for (char c : n)
      {
        t = t * 10 + (c - '0');
      }
    }
  }
};

void format_from(string_view is, string_view fmt, int &i)
{
  auto [found, bef, aft] = around(fmt, "{}"_sv);

  if (found)
  {
    if (starts_with(is, bef))
    {

      is = view(is.begin() + bef.size() , is.end());
      formatter2<int>::from(is, i);
    }
  }
}

int main()
{
  int i;
  format_from("coucou12world!", "coucou{}world!", i);
  cout.print(i);
  return 0;
}
