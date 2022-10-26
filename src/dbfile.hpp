#ifndef __stew_dbfile_hpp__
#define __stew_dbfile_hpp__

#include <filesystem>
#include <string_view>
#include <optional>

namespace stew::dbf
{
  namespace fs
  {
    enum class error
    {
      none,
      file_not_found
    };

    template <typename T>
    error select(std::optional<T> &op, std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }

    template <typename T>
    error update(T &t, std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }

    template <typename T>
    error insert(T &t, std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }

    error remove(std::string_view path)
    {
      namespace fs = std::filesystem;

      if (fs::exists(path))
      {
        return error::none;
      }
      else
      {
        return error::file_not_found;
      }
    }
  }

  namespace query
  {
    
  }

}

#endif