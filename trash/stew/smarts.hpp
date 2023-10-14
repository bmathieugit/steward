#ifndef __steward_smarts_hpp__
#define __steward_smarts_hpp__

#include <stew/meta.hpp>
#include <stew/utils.hpp>

namespace stew {
template <typename T>
class non_owning {
 private:
  rm_array<T> *_ptr = nullptr;

 public:
  constexpr ~non_owning() = default;
  constexpr non_owning() = default;

  template <typename U>
  constexpr non_owning(U *ptr) : _ptr(ptr) {}

  constexpr non_owning(nullptr_t) : non_owning() {}

  constexpr non_owning(const non_owning &) = default;
  constexpr non_owning(non_owning &&) = default;

  template <typename U>
  constexpr non_owning &operator=(U *ptr) {
    if (_ptr != ptr) {
      _ptr = ptr;
    }

    return *this;
  }

  constexpr non_owning &operator=(nullptr_t) {
    _ptr = nullptr;
    return *this;
  }

  constexpr non_owning &operator=(const non_owning &) = default;
  constexpr non_owning &operator=(non_owning &&) = default;

 public:
  constexpr auto get() { return _ptr; }

  auto get() const { return _ptr; }

 public:
  constexpr operator bool() const { return _ptr != nullptr; }

  constexpr auto operator*() -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return (*_ptr);
  }

  constexpr auto operator*() const -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return (*_ptr);
  }

  constexpr auto operator->() -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return _ptr;
  }

  constexpr auto operator->() const -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return _ptr;
  }

  constexpr auto operator[](size_t i) -> decltype(auto)
    requires(native_array_like<T>)
  {
    return (_ptr[i]);
  }

  constexpr auto operator[](size_t i) const -> decltype(auto)
    requires(native_array_like<T>)
  {
    return (_ptr[i]);
  }
};

template <typename T>
class owning {
 private:
  rm_array<T> *_ptr = nullptr;

 private:
  void cleanup() {
    if (_ptr != nullptr) {
      if constexpr (native_array_like<T>) {
        delete[] _ptr;
      } else {
        delete _ptr;
      }

      _ptr = nullptr;
    }
  }

 public:
  ~owning() { cleanup(); }

  owning() = default;

  owning(rm_array<T> *ptr) : _ptr(ptr) {}
  owning(const owning &) = delete;
  owning(owning &&o) : _ptr(transfer(o._ptr)) {}

  owning &operator=(const owning &) = delete;
  owning &operator=(owning &&o) {
    assert(this != &o);
    cleanup();
    _ptr = transfer(o._ptr);
    return *this;
  }

  owning &operator=(rm_array<T> *ptr) {
    assert(_ptr != ptr);
    cleanup();
    _ptr = ptr;
    return *this;
  }

 public:
  auto get() const { return _ptr; }

 public:
  auto operator*() const -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return (*_ptr);
  }

  auto operator->() const -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return _ptr;
  }

  auto operator[](size_t i) const -> decltype(auto)
    requires native_array_like<T>
  {
    return _ptr[i];
  }
};

template <typename T>
class local {
 private:
  rm_array<T> *_ptr = nullptr;

 public:
  constexpr ~local() {
    if (_ptr != nullptr) {
      if constexpr (native_array_like<T>) {
        delete[] _ptr;
      } else {
        delete _ptr;
      }

      _ptr = nullptr;
    }
  }

  constexpr local() = default;
  constexpr local(rm_array<T> *ptr) : _ptr(ptr) {}
  constexpr local(const local &) = delete;
  constexpr local(local &&) = delete;
  constexpr local &operator=(const local &) = delete;
  constexpr local &operator=(local &&) = delete;

 public:
  constexpr auto get() const { return _ptr; }

 public:
  constexpr auto operator*() const -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return (*_ptr);
  }

  constexpr auto operator->() const -> decltype(auto)
    requires(!native_array_like<T>)
  {
    return _ptr;
  }

  constexpr auto operator[](size_t i) const -> decltype(auto)
    requires native_array_like<T>
  {
    return _ptr[i];
  }
};

}  // namespace stew

#endif
