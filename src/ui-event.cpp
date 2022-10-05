#include <ui-event.hpp>

#include <thread>

namespace stew::ui
{
  void bus::push(const std::string &topic, const message &mess)
  {
    _topics[topic].push(mess);
  }

  void bus::push(const std::string &topic, message &&mess)
  {
    _topics[topic].push(std::move(mess));
  }

  std::optional<message> bus::pop(const std::string &topic)
  {
    if (!_topics[topic].empty())
    {
      message mess = std::move(_topics[topic].front());
      _topics[topic].pop();
      return mess;
    }
    else
    {
      return std::nullopt;
    }
  }

  consumer::consumer(bus &bs, const std::string& topic)
      : _bus(bs), _topic(topic) {}

  std::optional<message> consumer::consume()
  {
    return _bus.pop(_topic);
  }

  producer::producer(bus &bs, const std::string& topic)
      : _bus(bs), _topic(topic) {}

  void producer::produce(const message &mess)
  {
    _bus.push(_topic, mess);
  }

  void producer::produce(message &&mess)
  {
    _bus.push(_topic, std::move(mess));
  }
}