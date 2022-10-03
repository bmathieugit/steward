#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

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
    std::map<std::string, std::vector<message>> _topics;
    std::map<std::uuid, std::map<std::string, std::size_t>> _conso;

  public:
    void send(const std::string &topic, const message &mess);
    void send(const std::string &topic, message &&mess);

    std::optional<message> receive(const std::string &topic, std::uuid id);

  public:
    void purge(const std::string &topic);
    void purge();
  };

  class consumer
  {
    bus &_bus;
    std::uuid _uuid = 0;
    std::function<void(const message &)> _cb;

  public:
    consumer(bus &bs, std::function<void(const message &)> &&cb);

  public:
    bool consume(const std::string &topic);
  };

  class producer
  {
    bus &_bus;

  public:
    producer(bus &bs);

  public:
    void produce(const std::string &topic, const message &mess);
    void produce(const std::string &topic, message &&mess);
  };

  class loop
  {
    bus &_bus;
    bool _continue = true;
    std::map<std::string, std::vector<consumer>> _subscriptions;

  public:
    loop(bus &bs);

  public:
    void subscribe(const std::string &topic, const consumer &cons);
    void run();

  private:
    void turn();
  };

}

#endif