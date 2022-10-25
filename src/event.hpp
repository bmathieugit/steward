#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <optional>
#include <functional>
#include <map>
#include <string>
#include <list>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace stew
{
  template <typename M>
  std::scoped_lock<M> make_scoped(M &mutex)
  {
    return std::scoped_lock<M>(mutex);
  }

  template <typename M>
  std::unique_lock<M> make_uniquelk(M &mutex)
  {
    return std::unique_lock<M>(mutex);
  }

  class message
  {
    std::map<std::string, std::string> _data;

  public:
    ~message() = default;
    message() = default;
    message(const message &) = default;
    message(message &&) = default;
    message &operator=(const message &) = default;
    message &operator=(message &&) = default;

  public:
    bool contains(const std::string &key) const;
    std::optional<std::string> get(const std::string &key) const;
    void append(const std::string &key, const std::string &val);
  };

  class subject
  {
  public:
    using observer = std::function<void(const message &)>;

  private:
    std::list<observer> _observers;

  public:
    std::list<observer> &observers();
    void attach(observer obs);
    void update(const message &mess);
  };

  class subscriber
  {
    std::mutex _mutex;
    std::condition_variable _empty_guard;
    std::queue<message> _messages;
    bool _closed = false;

  public:
    subscriber() = default;
    subscriber(const subscriber &) = delete;
    subscriber(subscriber &&o) = default;
    subscriber &operator=(const subscriber &) = delete;
    subscriber &operator=(subscriber &&) = default;

  public:
    std::optional<message> consume();
    bool closed();
    bool can_consume_again();

    friend class topic;
  };

  class topic
  {
    std::mutex _mutex;
    std::list<subscriber> _subscribers;

  public:
    topic() = default;
    topic(const topic &) = delete;
    topic(topic &&) = default;
    topic &operator=(const topic &) = delete;
    topic &operator=(topic &&) = default;

  public:
    subscriber &subscribe();
    void post(const message &mess);
    void close();
  };
}

#endif