#include <ui-config.hpp>

namespace std
{

  std::uuid next_uuid()
  {
    static std::uuid id = 0;
    return ++id;
  }
}