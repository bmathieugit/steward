#ifndef __stew_exceptions_hpp__
#define __stew_exceptions_hpp__

#include <exception>

namespace std
{
  class null_pointer_access : public std::exception
  {
  public:
    virtual ~null_pointer_access() noexcept = default;
    virtual const char *what() const noexcept override;
  };

  const char *null_pointer_access::what() const noexcept
  {
    return "error on null pointer access";
  }
}

#endif