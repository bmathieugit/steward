#include <stew.hpp>

using namespace stew;

// Ce que je vais faire maintenant c'est de développer un système de gestion des évenements
// le plus basique possible afin de rester aussi près que possible des performances optimales.
// Nous allons partir sur une API type signal/slot sur des types strict. L'héritage ne
// sera pas fonctionnel sur les évènements.

template <typename T>
class observer
{
private:
  T _t;

private:
  function<void(const T &)> _destroyed;
  function<void(const T &)> _changed;

public:
  ~observer()
  {
    if (!_destroyed.empty())
    {
      _destroyed(_t);
    }
  }

  observer(const T &t) : _t(t) {}
  observer(T &&t) : _t(t) {}

  observer(const observer &) = default;
  observer(observer &&) = default;

  observer &operator=(const T &t)
  {
    _t = t;

    if (!_changed.empty())
    {
      _changed(_t);
    }

    return *this;
  }

  observer &operator=(T &&t)
  {
    _t = transfer(t);

    if (!_changed.empty())
    {
      _changed(_t);
    }

    return *this;
  }

  observer &operator=(const observer &) = default;
  observer &operator=(observer &&) = default;

public:
  enum class signal
  {
    destroyed,
    changed
  };

  template <signal S, callable<void, const int&> F>
  void slot(F&& sl)
  {
    if constexpr (S == signal::destroyed)
    {
      _destroyed = forward<F>(sl);
    }
    else if constexpr (S == signal ::changed)
    {
      _changed = forward<F>(sl);
    }
  }
};

void foo(const int &i)
{
  printf("coucou\n");
}

struct foo2
{
  void operator()(const int &i)
  {
    printf("coucou2\n");
  }
};

int main()
{
  using oi = observer<int>;

  observer<int> i = 12;

  i.slot<oi::signal::changed>([](const int& i) {
    console<char>::printfln("coucou3{}{}", 12, true);
  });

  i = 24;

  return 0;
}
