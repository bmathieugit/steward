#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <ui-config.hpp>

namespace stew::ui
{
  template <typename M>
  std::scoped_lock<M> make_scoped(M &mutex)
  {
    return std::scoped_lock<M>(mutex);
  }

  struct message
  {
    std::string _data;
  };

  class subscriber
  {
    std::mutex _mutex;
    std::queue<message> _messages;

  public:
    subscriber() = default;
    subscriber(const subscriber &) = delete;
    subscriber(subscriber && o) : 
      _mutex(std::move(o._mutex))
    {

    }
    subscriber &operator=(const subscriber &) = delete;
    subscriber &operator=(subscriber &&) = default;

  public:
    std::optional<message> consume()
    {
      auto scoped = make_scoped(_mutex);

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

    friend class topic;
  };

  class topic
  {
    std::mutex _mutex;
    std::vector<subscriber> _subscribers;

  public:
    subscriber &subscribe()
    {
      {
        auto scoped = make_scoped(_mutex);
        _subscribers.emplace_back();
      }

      return _subscribers.back();
    }

    void post(const message &mess)
    {
      auto scoped = make_scoped(_mutex);

      for (auto &cons : _subscribers)
      {
        auto scoped2 = make_scoped(_mutex);
        cons._messages.push(mess);
      }
    }

    void post(message &&mess)
    {
      auto scoped = make_scoped(_mutex);

      for (auto &cons : _subscribers)
      {
        auto scoped2 = make_scoped(_mutex);
        cons._messages.push(std::move(mess));
      }
    }
  };
}

#endif