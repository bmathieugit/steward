#include <stew.hpp>

using namespace stew;

namespace ansi {

constexpr auto clear = str::view("\033[2J");
constexpr auto init = str::view("\033[H");
constexpr auto reset = str::view("\033[0m");
constexpr auto bold = str::view("\033[1m");
constexpr auto bg_red = str::view("\033[41m");
constexpr auto fg_red = str::view("\033[31m");
}  // namespace ansi

namespace prefix {

constexpr auto t1 = str::view("t1=");
constexpr auto t2 = str::view("t2=");
constexpr auto t3 = str::view("t3=");
constexpr auto t4 = str::view("t4=");
constexpr auto p = str::view("p=");
constexpr auto ns = str::view("===");

}  // namespace prefix

namespace slide {
class line {
 private:
  string_view<char> _content;

 public:
  ~line() = default;
  line(string_view<char> content) : _content(content) {}
  line(const line&) = default;
  line(line&&) = default;
  line& operator=(const line&) = default;
  line& operator=(line&&) = default;

 public:
  string_view<char> content() const { return _content; }
};

class slide {
 private:
  string_view<char> _content;
  string_view<char> _tail;

 public:
  ~slide() = default;
  slide(string_view<char> content) : _content(content), _tail(_content) {}
  slide(const slide&) = default;
  slide(slide&&) = default;
  slide& operator=(const slide&) = default;
  slide& operator=(slide&&) = default;

 public:
  auto content() const { return _content; }
  maybe<line> next() { 
    maybe<line> l; 
    
    if (!_tail.empty()){
      auto fnd = find(_tail, '\n');
      auto tmp = string_view<char>(_tail.begin(), fnd);

      
    }
 
    return l;}
};

class slides {
 private:
  fixed_string<char> _content;
  string_view<char> _tail;
  fixed_string<char> _config;
  size_t _size = 0;
  size_t _curr = 0;

 public:
  ~slides() = default;

  explicit slides(fixed_string<char>&& content, fixed_string<char>&& config)
      : _content(transfer(content)),
        _tail(_content),
        _config(transfer(config)),
        _size(stew::count(_content, prefix::ns) - 1) {}

  slides(const slides&) = default;
  slides(slides&&) = default;
  slides& operator=(const slides&) = default;
  slides& operator=(slides&&) = default;

 public:
  size_t size() const { return _size; }
  size_t curr() const { return _curr; }

  tuple<size_t, size_t> page() const {
    return tuple<size_t, size_t>(_curr, _size);
  }

 public:
  maybe<slide> next() {
    maybe<slide> res;

    if (!_tail.empty() && starts_with(_tail, prefix::ns)) {
      _tail = string_view<char>(_tail.begin() + prefix::ns.size(), _tail.end());

      auto fnd = stew::find(_tail, prefix::ns);
      auto tmp = string_view<char>(_tail.begin(), fnd);

      if (!(tmp.empty() || stew::all_of(tmp, [](char c) {
              return c == ' ' || c == '\n' || c == '\t';
            }))) {
        res = slide(string_view<char>(_tail.begin(), fnd));
        _tail = string_view<char>(fnd, _tail.end());
        ++_curr;
      }
    }

    return res;
  }
};
}  // namespace slide

int main(int argc, char** argv) {
  if (argc == 2) {
    static_string<char, 1024> buff;
    file<char, mode::r> fslides(str::view(argv[1]));

    if (fslides.opened()) {
      slide::slides sls(io::readall(fslides), str::fixed(""));
      maybe<slide::slide> sl;
      maybe<slide::line> l;

      while ((sl = sls.next()).has()) {
        io::printfln(str::view("slide $/$"), sls.curr(), sls.size());

        while ((l = (*sl).next()).has()) {
          io::printfln(str::view(" -- content : \n$"), (*l).content());
        }
      }
    }
  }

  return 0;
}
