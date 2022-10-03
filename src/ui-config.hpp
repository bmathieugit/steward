#ifndef __stew_ui_config_hpp__
#define __stew_ui_config_hpp__

#include <memory>

namespace std
{
  template <typename T>
  using ptr = unique_ptr<T>;

  using uuid = std::size_t;

  std::uuid next_uuid();
}

#endif