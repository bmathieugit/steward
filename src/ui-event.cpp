#include <ui-event.hpp>

#include <thread>

namespace stew::ui
{
  void bus::send(const std::string &topic, const message &mess)
  {
    std::scoped_lock<std::mutex> sl(_guardian);
    _topics[topic].push(mess);
  }

  void bus::send(const std::string &topic, message &&mess)
  {
    std::scoped_lock<std::mutex> sl(_guardian);
    _topics[topic].push(std::move(mess));
  }

  std::optional<message> bus::receive(const std::string &topic)
  {
    std::scoped_lock<std::mutex> sl(_guardian);

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
    return _bus.receive(_topic);
  }

  producer::producer(bus &bs, const std::string& topic)
      : _bus(bs), _topic(topic) {}

  void producer::produce(const message &mess)
  {
    _bus.send(_topic, mess);
  }

  void producer::produce(message &&mess)
  {
    _bus.send(_topic, std::move(mess));
  }
}