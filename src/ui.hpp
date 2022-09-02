#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace stew::ui
{
  template <typename T>
  concept ui_widget = requires(T &t)
  {
    t.render();
  };

  class text_input
  {
    std::string label;

  public:
    ~text_input() = default;
    text_input(std::string_view _label)
        : label(_label) {}
    text_input(const text_input &) = default;
    text_input(text_input &&) = default;
    text_input &operator=(const text_input &) = default;
    text_input &operator=(text_input &&) = default;

  public:
    void render()
    {
      std::cout << "|---------------------------\n";
      std::cout << "| " << label << ": ";
    }

    void capture(std::string &v)
    {
      std::cin >> v;
      std::cout << "|---------------------------\n";
    }
  };

  class table
  {
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;

  public:
    ~table() = default;
    explicit table(const std::vector<std::string> &_headers)
        : headers(_headers) {}
    table() = default;
    table(const table &) = default;
    table(table &&) = default;
    table &operator=(const table &) = default;
    table &operator=(table &&) = default;

  public:
    void push_row(const std::vector<std::string> &row)
    {
      if (!headers.empty() && row.size() == headers.size())
        rows.push_back(row);
    }

    void render()
    {
      std::vector<std::size_t> sizes = columns_max_size();

      for (std::size_t i(0); i < sizes.size(); ++i)
        std::cout << '|' << std::string(sizes[i] + 2, '-') << '|';
      std::cout << '\n';

      for (std::size_t i(0); i < sizes.size(); ++i)
        std::cout << '|' << ' ' << headers[i] << std::string(sizes[i] - headers[i].size() + 1, ' ') << '|';
      std::cout << '\n';

      for (std::size_t i(0); i < sizes.size(); ++i)
        std::cout << '|' << std::string(sizes[i] + 2, '-') << '|';
      std::cout << '\n';

      for (std::size_t l(0); l < rows.size(); ++l)
      {
        for (std::size_t i(0); i < headers.size(); ++i)
          std::cout << '|' << ' ' << rows[l][i] << std::string(sizes[i] - rows[l][i].size() + 1, ' ') << '|';
        std::cout << '\n';
      }
    }

  private:
    std::vector<std::size_t> columns_max_size()
    {
      std::vector<size_t> sizes(headers.size());

      for (std::size_t i(0); i < headers.size(); ++i)
        sizes[i] = headers[i].size();

      for (std::size_t l(0); l < rows.size(); ++l)
        for (std::size_t i(0); i < headers.size(); ++i)
          if (sizes[i] < rows[l][i].size())
            sizes[i] = rows[l][i].size();

      return sizes;
    }
  };
}

#endif