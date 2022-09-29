#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <map>

#include <iostream>

namespace stew::ui
{
  struct message
  {
    std::string _topic;
    std::string _data;
  };

  class dispatcher
  {
    using consumer = std::function<void(const message &)>;
    using aggregator = std::function<void(const std::vector<message> &)>;

    std::map<std::string, std::vector<message>> _topics;
    std::map<std::string, std::vector<consumer>> _consumers;
    std::map<std::string, std::vector<aggregator>> _aggregators;

  public:
    void connect(std::string_view topic, const consumer &cons);
    void connect(std::string_view topic, consumer &&cons);
    void connect(std::string_view topic, const aggregator &aggr);
    void connect(std::string_view topic, aggregator &&aggr);

  public:
    void publish(const message &mess);
    void publish(message &&mess);

  public:
    void consume();
  };

  class bus
  {
    dispatcher *_disp;

  public:
    bus(dispatcher *disp);

  public:
    void emit(const message &mess);
    void emit(message &&mess);
  };

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

#endif