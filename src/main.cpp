#define NDEBUG

#include <stew.hpp>

using namespace stew;

int main()
{
  extract_response<int, int> resp;

  string_view<char> input = "hello 12, 12!";

  extract_to(input, "hello \0, \0!", resp.get<0>(), resp.get<1>());
  console<char>::printf("\0 \0"_sv, *resp.get<0>(), *resp.get<1>());


  return 0;
}
