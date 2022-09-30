#ifndef __stew_ui_event_hpp__
#define __stew_ui_event_hpp__

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

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
    dispatcher &_disp;

  public:
    bus(dispatcher &disp);

  public:
    void emit(const message &mess);
    void emit(message &&mess);
  };
}

#endif