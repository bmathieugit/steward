#ifndef __stew_csv_hpp__
#define __stew_csv_hpp__

#include <string>
#include <string_view>

#include <sstream>
#include <fstream>

namespace stew::csv
{

  template <typename T>
  struct csv_entry;

  struct csv_marshaller
  {
  public:
    template <typename T>
    std::string marshall(const T &t) const
    {
      std::stringstream ss;
      csv_entry<T>().from(
          t,
          [this, &ss](const auto &...i)
          { this->marshall_entry(ss, i...); });
      return ss.str();
    }

    template <typename H, typename... R>
    void marshall_entry(
        std::stringstream &ss,
        const H &h, const R &...r) const
    {
      ss << '"' << h << '"';

      if constexpr (sizeof...(R) > 0)
      {
        ss << ';';
        marshall_entry(ss, r...);
      }
    }

  public:
    template <typename T>
    T unmarshall(std::string_view s) const
    {
      T t;
      csv_entry<T>().to(t, [this, s](auto &...i)
                        { this->unmarshall_entry(s, i...); });
      return t;
    }

    template <typename H, typename... R>
    void unmarshall_entry(
        std::string_view line, H &h, R &...r) const
    {
      std::string_view item = line.substr(0, line.find(';'));

      if (item.size() > 2)
      {
        item.remove_prefix(1);
        item.remove_suffix(1);

        h = item;

        if constexpr (sizeof...(R))
        {
          line.remove_prefix(
              std::min(item.size() + 1,
                       line.size()));
          unmarshall_entry(line, r...);
        }
      }
    }
  };

  template <typename R>
  concept CRange = requires(const R &r)
  {
    std::begin(r);
    std::end(r);
  };

  template <typename STREAM>
  struct csv_stream
  {
    STREAM stream;

    template <typename T>
    void write_one(const T &t)
    {
      csv_marshaller csvm;
      stream << csvm.marshall(t);
    }

    template <CRange R>
    void write_all(const R &r)
    {
      for (const auto &item : r)
        write(item);
    }

    template <typename T>
    T read_one();
    {
    }

    template <typename T>
    std::vector<T> read_all()
    {
    }
  };

  using csv_fstream = csv_stream<std::fstream>;
}

#endif