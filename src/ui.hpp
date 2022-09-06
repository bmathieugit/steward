#ifndef __stew_ui_hpp__
#define __stew_ui_hpp__

#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>

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

  struct cursor_position
  {
    std::size_t x = 0;
    std::size_t y = 0;
  };

  struct ansi_navigator
  {
    std::ostream &os;

    void up(std::size_t n)
    {
      os << "\033[" << n << 'A';
    }

    void down(std::size_t n)
    {
      os << "\033[" << n << 'B';
    }

    void left(std::size_t n)
    {
      os << "\033[" << n << 'D';
    }

    void right(std::size_t n)
    {
      os << "\033[" << n << 'C';
    }
  };

  template <std::size_t X,
            std::size_t Y>
  class screen
  {
    std::array<std::array<char, X>, Y> buff;
    cursor_position cpos;

  public:
    ~screen() = default;
    screen()
    {
      clear();
    }

    screen(const screen &) = default;
    screen(screen &&) = default;
    screen &operator=(const screen &) = default;
    screen &operator=(screen &&) = default;

  public: // wirting methods
    void begin_write()
    {
      clear();
    }

    void end_write()
    {
    }

    void write(std::string_view s)
    {
      for (char c : s)
      {
        if (cpos.y < Y && cpos.x < X)
        {
          buff[cpos.y][cpos.x] = c;
        }
        else
        {
          break;
        }

        if ((c == '\n' || cpos.x == X - 1) && cpos.y < Y)
        {
          cpos.x = 0;
          cpos.y = cpos.y + 1;
        }
        else
        {
          cpos.x = cpos.x + 1;
        }
      }
    }

  private:
    void clear()
    {
      for (std::size_t y(0); y < Y; ++y)
        for (std::size_t x(0); x < X; ++x)
          buff[y][x] = '\0';
    }

  public: // cursor move position method
    cursor_position at_origin()
    {
      cpos.x = 0;
      cpos.y = 0;
      return cpos;
    }

    cursor_position at_next_marker()
    {
      while (cpos.x != X && cpos.y != Y)
      {
        if (cpos.x == X)
        {
          cpos.x = 0;
          cpos.y = cpos.y + 1;
        }
        else
        {
          cpos.x = cpos.x + 1;
        }

        char at = buff[cpos.y][cpos.x];

        if (at == '%')
        {
          break;
        }
      }

      return cpos;
    }

  public:
    void display()
    {
      std::cout << "\033[2J";
      std::cout << "\033[H";

      cursor_position pos;

      for (std::size_t y(0); y < Y; ++y)
      {
        for (std::size_t x(0); x < X; ++x)
        {
          if (buff[y][x] == '\0')
          {
            continue;
          }

          if (char c = buff[y][x]; c != '\n')
          {
            std::cout << c;
            pos.x = pos.x + 1;
          }
          else
          {
            std::cout << '\n';
            pos.x = 0;
            pos.y = pos.y + 1;
            break;
          }
        }
      }

      ansi_navigator nav{std::cout};

      if (pos.y != 0)
        nav.up(pos.y);

      if (pos.x != 0)
        nav.left(pos.x);

      if (cpos.x != 0)
        nav.right(cpos.x);

      if (cpos.y != 0)
        nav.down(cpos.y);
    }

  public:
    std::string capture()
    {
      std::string val;
      std::getline(std::cin >> std::ws, val);

      for (char c : val)
      {
        if (cpos.x < X - 1)
        {
          buff[cpos.y][cpos.x] = c;
          cpos.x = cpos.x + 1;
        }
      }

      buff[cpos.y][cpos.x] = '\n';

      cpos.x = 0;
      cpos.y = cpos.y + 1;
      return val;
    }
  };

  namespace form
  {
    struct input
    {
      std::string value;
      std::string label;

      template <
          std::size_t X,
          std::size_t Y>
      void render(screen<X, Y> &scr)
      {
        scr.write("--------------\n");
        scr.write("-- ");
        scr.write(label);
        scr.write(" : %\n");
      }
    };
  }

}

#endif