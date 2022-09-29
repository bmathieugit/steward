#include <ui-event.hpp>

namespace stew::ui
{
  void dispatcher::connect(std::string_view topic, const consumer &cons)
  {
    _consumers[std::string(topic)].push_back(cons);
  }

  void dispatcher::connect(std::string_view topic, consumer &&cons)
  {
    _consumers[std::string(topic)].push_back(std::move(cons));
  }

  void dispatcher::connect(std::string_view topic, const aggregator &aggr)
  {
    _aggregators[std::string(topic)].push_back(aggr);
  }

  void dispatcher::connect(std::string_view topic, aggregator &&aggr)
  {
    _aggregators[std::string(topic)].push_back(std::move(aggr));
  }

  void dispatcher::publish(const message &mess)
  {
    _topics[mess._topic].push_back(mess);
  }

  void dispatcher::publish(message &&mess)
  {
    _topics[mess._topic].push_back(std::move(mess));
  }

  void dispatcher::consume()
  {
    for (auto &&[topic, consumers] : _consumers)
    {
      for (consumer &cons : consumers)
      {
        for (const message &mess : _topics[topic])
        {
          cons(mess);
        }
      }
    }

    for (auto &&[topic, aggregators] : _aggregators)
    {
      for (aggregator &aggr : aggregators)
      {
        aggr(_topics[topic]);
      }
    }

    _topics.clear();
  }

  bus::bus(dispatcher *disp)
      : _disp(disp) {}

  void bus::emit(const message &mess)
  {
    if (_disp != nullptr)
    {
      _disp->publish(mess);
    }
  }

  void bus::emit(message &&mess)
  {
    if (_disp != nullptr)
    {
      _disp->publish(std::move(mess));
    }
  }
}