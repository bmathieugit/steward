#include <ui-event.hpp>

#include <thread>




namespace stew::ui_old
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

  bool bus::empty(const std::string &topic)
  {
    return _topics[topic].empty();
  }

  bus_guard::bus_guard(bus &bs)
      : _bus(bs) {}

  void bus_guard::push(const std::string &topic, const message &mess)
  {
    _mutex.lock();
    _bus.push(topic, mess);
    _mutex.unlock();
    _cv.notify_all();
  }

  void bus_guard::push(const std::string &topic, message &&mess)
  {
    _mutex.lock();
    _bus.push(topic, std::move(mess));
    _mutex.unlock();
    _cv.notify_all();
  }

  std::optional<message> bus_guard::pop(const std::string &topic)
  {
    std::unique_lock<std::mutex> ul(_mutex);
    _cv.wait(ul,  [&topic, this]
             { return !this->_bus.empty(topic); });
    return _bus.pop(topic);
  }

  consumer::consumer(bus &bs, const std::string &topic)
      : _bus(bs), _topic(topic) {}

  std::optional<message> consumer::consume()
  {
    return _bus.pop(_topic);
  }

  producer::producer(bus &bs, const std::string &topic)
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