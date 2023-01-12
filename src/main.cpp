// #include <dbfile.hpp>

#include <stew.hpp>
#include <iterator>

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

namespace old
{
  void format_from(string_view is, string_view fmt, int &i)
  {
    auto [found, bef, aft] = around(fmt, "{}"_sv);

    if (found)
    {
      if (starts_with(is, bef))
      {

        is = view(is.begin() + bef.size(), is.end());
        formatter2<int>::from(is, i);
      }
    }
  }
}

template <typename I>
concept char_istream =
    requires(I &i, char &c) { {i.pop(c)} -> convertible_to<size_t>; };

template <typename I>
concept wchar_istream =
    requires(I &i, wchar_t &c) { {i.pop()} -> convertible_to<size_t>; };

template <typename I>
concept istream = char_istream<I> || wchar_istream<I>;

template <character C, istream I>
class istream_iterator
{
private:
  C _c;
  non_owning<I> _is;

public:
  constexpr istream_iterator(I *is)
      : _is(is)
  {
    if (!_is)
    {
      if (_is.get()->pop(_c) == 0)
      {
        _is = nullptr;
      }
    }
  }

  constexpr istream_iterator() = default;

public:
  constexpr istream_iterator &operator++()
  {
    if (!_is || _is.get()->pop(_c) == 0)
    {
      _is = nullptr;
    }

    return *this;
  }

  constexpr istream_iterator operator++(int)
  {
    auto copy = *this;
    ++(*this);
    return copy;
  }

  constexpr auto operator==(const istream_iterator &o)
  {
    return _c == o._c && _is == o._is;
  }

  constexpr auto operator*() const
  {
    return _c;
  }
};

template <typename... T>
void format_from(string_view is, string_view fmt, T &...t)
{
  // Il faut que je puisse parcourir is une et une seule fois. je vais donc
  // mettre au point un iterator sur istream permettant de parcourir un istream.
}

int main()
{
  string is = "coucou tout le monde !!!";

  istream_iterator<char, string> b(&is);
  istream_iterator<char, string> e;

  for (auto c : view(b, e))
  {
    cout.print(c);
  }

  int i;
  format_from("coucou12world!", "coucou{}world!", i);
  cout.print(i);
  return 0;
}
