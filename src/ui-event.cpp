#include <ui-event.hpp>

namespace stew::ui
{
  void bus::send(const std::string &topic, const message &mess)
  {
    _topics[topic].push_back(mess);
  }

  void bus::send(const std::string &topic, message &&mess)
  {
    _topics[topic].push_back(std::move(mess));
  }

  std::optional<message> bus::receive(const std::string &topic, std::uuid id)
  {
    if (!_conso[id].contains(topic))
    {
      _conso[id][topic] = 0;
    }

    std::size_t idx = _conso[id][topic];

    if (idx < _topics[topic].size())
    {
      _conso[id][topic] = idx + 1;
      return _topics[topic][idx];
    }
    else
    {
      return std::nullopt;
    }
  }

  void bus::purge(const std::string &topic)
  {
    _topics[topic].clear();

    for (auto &[id, topics] : _conso)
    {
      topics.erase(topic);
    }
  }

  void bus::purge()
  {
    _topics.clear();
    _conso.clear();
  }

  consumer::consumer(bus &bs, std::function<void(const message &)> &&cb)
      : _cb(cb), _bus(bs), _uuid(std::next_uuid()) {}

  bool consumer::consume(const std::string &topic)
  {
    std::optional<message> mess = _bus.receive(topic, _uuid);

    if (mess.has_value())
    {
      _cb(mess.value());
      return true;
    }
    else
    {
      return false;
    }
  }

  producer::producer(bus &bs)
      : _bus(bs) {}

  void producer::produce(const std::string &topic, const message &mess)
  {
    _bus.send(topic, mess);
  }

  void producer::produce(const std::string &topic, message &&mess)
  {
    _bus.send(topic, std::move(mess));
  }

  loop::loop(bus &bs)
      : _bus(bs) {}

  void loop::subscribe(const std::string &topic, const consumer &cons)
  {
    _subscriptions[topic].push_back(cons);
  }

  void loop::run()
  {
    int i = 0;

    while (i < 1000)
    {
      turn();
      ++i;
    }
  }

  void loop::turn()
  {
    for (auto &[topic, consos] : _subscriptions)
    {
      for (consumer &conso : consos)
      {
        while (conso.consume(topic));
      }
    }

    _bus.purge();
  }
}