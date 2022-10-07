#include <ui-event.hpp>

namespace stew::ui
{
  std::optional<message> subscriber::consume()
  {
    _mutex.lock();

    if (_messages.empty() && _closed)
    {
      _mutex.unlock();
      return std::nullopt;
    }
    else
    {
      _mutex.unlock();

      auto uniquelk = make_uniquelk(_mutex);
      _empty_guard.wait(uniquelk, [this] { return !this->_messages.empty(); });

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