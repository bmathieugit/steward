#include <stew.hpp>

using namespace stew;

// Ce que je vais faire maintenant c'est de développer un système de gestion des évenements
// le plus basique possible afin de rester aussi près que possible des performances optimales.
// Nous allons partir sur une API type signal/slot sur des types strict. L'héritage ne
// sera pas fonctionnel sur les évènements.

enum class signal
{
  pre_changed,
  changed,
  pre_destroyed,
  destroyed
};

template <typename T>
class observer2;

template <typename T>
class observable
{
private:
  T _t;
  vector<observer2<T> *> _obss;

public:
  ~observable()
  {
    notify<signal::pre_destroyed>();
  }

  observable(const T &t) : _t(t) {}
  observable(T &&t) : _t(t) {}

  observable(const observable &) = default;
  observable(observable &&) = default;

  observable &operator=(const T &t)
  {
    notify<signal::pre_changed>();
    _t = t;
    notify<signal::changed>();
    return *this;
  }

  observable &operator=(T &&t)
  {
    notify<signal::pre_changed>();
    _t = transfer(t);
    notify<signal::changed>();
    return *this;
  }

  observable &operator=(const observable &) = default;
  observable &operator=(observable &&) = default;

private:
  template <signal S>
  void notify()
  {
    for (observer2<T> *obs : _obss)
    {
      if (obs != nullptr)
      {
        obs->template notify<S>(_t);
      }
    }
  }

  void observe(observer2<T> *obs)
  {
    _obss.push(obs);
  }

  friend class observer2<T>;
};

template <typename T>
class observer2
{
private:
  reference<observable<T>> _obs;
  function<void(const T &)> _pre_destroyed;
  function<void(const T &)> _pre_changed;
  function<void(const T &)> _changed;

public:
  ~observer2() = default;
  observer2(observable<T> &obs) : _obs(obs)
  {
    obs.observe(this);
  }

  observer2(const observer2 &) = delete;
  observer2(observer2 &&) = default;
  observer2 &operator=(const observer2 &) = delete;
  observer2 &operator=(observer2 &&) = default;

public:
  template <signal S>
  void notify(const T &t)
  {

    if constexpr (S == signal::pre_changed)
    {
      if (!_pre_changed.empty())
      {
        _pre_changed(t);
      }
    }
    else if constexpr (S == signal::changed)
    {
      if (!_changed.empty())
      {
        _changed(t);
      }
    }
    else if constexpr (S == signal::pre_destroyed)
    {
      if (!_pre_destroyed.empty())
      {
        _pre_destroyed(t);
      }
    }
  }

  template <signal S, callable<void, const int &> F>
  void slot(F &&sl)
  {
    if constexpr (S == signal::pre_destroyed)
    {
      _pre_destroyed = forward<F>(sl);
    }
    else if constexpr (S == signal::pre_changed)
    {
      _pre_changed = forward<F>(sl);
    }
    else if constexpr (S == signal ::changed)
    {
      _changed = forward<F>(sl);
    }
  }
};

int main()
{
  atomic<int> i(20);
  i.apply([](int &t) { t = 25; });
  i.apply([](const int &t) { console<char>::printfln("nouvelle valeur : {}", t); });

  return 0;
}
