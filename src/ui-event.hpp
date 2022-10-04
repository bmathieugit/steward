#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <queue>
#include <mutex>

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
    std::mutex _guardian;
    std::map<std::string, std::queue<message>> _topics;

  public:
    void send(const std::string &topic, const message &mess);
    void send(const std::string &topic, message &&mess);

    std::optional<message>
    receive(const std::string &topic);

  public:
    void purge(const std::string &topic);
    void purge();
  };

  class consumer
  {
    bus &_bus;
    std::string _topic;

  public:
    consumer(bus &bs, const std::string& topic);

  public:
    std::optional<message> consume();
  };

  class producer
  {
    bus &_bus;
    std::string _topic;

  public:
    producer(bus &bs, const std::string& topic);

  public:
    void produce(const message &mess);
    void produce(message &&mess);
  };
}

#endif