#include <event.hpp>

namespace stew
{

  bool message::contains(const std::string &key) const
  {
    return _data.contains(key);
  }

  std::optional<std::string> message::get(const std::string &key) const
  {
    auto found = _data.find(key);

    if (found == _data.end())
    {
      return std::nullopt;
    }
    else
    {
      return std::optional<std::string>((*found).second);
    }
  }

  void message::append(const std::string &key, const std::string &val)
  {
    _data[key] = val;
  }

  std::list<subject::observer> &subject::observers()
  {
    return _observers;
  }

  void subject::attach(subject::observer obs)
  {
    _observers.push_back(std::move(obs));
  }

  void subject::update(const message &mess)
  {
    for (observer &obs : _observers)
    {
      obs(mess);
    }
  }

  std::optional<message> subscriber::consume()
  {
    {
      make_scoped(_mutex);

      if (_messages.empty() && _closed)
      {
        return std::nullopt;
      }
    }

    auto uniquelk = make_uniquelk(_mutex);
    _empty_guard.wait(uniquelk, [this]
                      { return !this->_messages.empty(); });

    if (!_messages.empty())
    {
      auto mess = std::move(_messages.front());
      _messages.pop();
      return mess;
    }
    else
    {
      return std::nullopt;
    }
  }

  subscriber &topic::subscribe()
  {
    auto scoped = make_scoped(_mutex);
    _subscribers.emplace_back();
    return _subscribers.back();
  }

  void topic::post(const message &mess)
  {
    auto scoped = make_scoped(_mutex);

    for (auto &cons : _subscribers)
    {
      auto scoped2 = make_scoped(cons._mutex);
      cons._messages.push(mess);
      cons._empty_guard.notify_one();
    }
  }

  void topic::close()
  {
    auto scoped = make_scoped(_mutex);

    for (auto &cons : _subscribers)
    {
      auto scoped2 = make_scoped(cons._mutex);
      cons._closed = true;
    }
  }
}