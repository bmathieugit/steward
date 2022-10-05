#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

#include <ui-config.hpp>

namespace stew::ui
{
  struct message
  {
    std::string _topic;
    std::string _data;
  };

  class bus
  {
    std::map<std::string, std::queue<message>> _topics;

  public:
    void push(const std::string &topic, const message &mess);
    void push(const std::string &topic, message &&mess);

    std::optional<message>
    pop(const std::string &topic);

    bool empty(const std::string& topic);
  };

  class bus_guard
  {
    // un bus guard permet de poser un mutex sur les topics afin de
    // protéger chaque topic et de permettre de ne lancer la lecture que s'il y a un
    // event dans le topic.
    std::mutex _mutex;
    std::condition_variable _cv;
    bus &_bus;

  public:
    bus_guard(bus &bs);

    void push(const std::string &topic, const message &mess);
    void push(const std::string &topic, message &&mess);

    std::optional<message> pop(const std::string &topic);
  };

  class consumer
  {
    bus &_bus;
    std::string _topic;

  public:
    consumer(bus &bs, const std::string &topic);

  public:
    std::optional<message> consume();
  };

  class producer
  {
    bus &_bus;
    std::string _topic;

  public:
    producer(bus &bs, const std::string &topic);

  public:
    void produce(const message &mess);
    void produce(message &&mess);
  };
}

#endif