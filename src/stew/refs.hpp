#ifndef __steward_references_hpp__
#define __steward_references_hpp__

#include <stew/smarts.hpp>

namespace stew {

template <typename T>
class reference {
 private:
  non_owning<T> _t;

 public:
  constexpr ~reference() = default;
  constexpr reference(T &t) : _t(&t) {}
  constexpr reference(const reference &) = default;
  constexpr reference &operator=(const reference &) = default;

 public:
  constexpr operator T &() const { return *_t; }

  constexpr T &get() const { return *_t; }
};

template <typename T>
class const_reference {
 private:
  non_owning<const T> _t;

 public:
  constexpr ~const_reference() = default;
  constexpr const_reference(const T &t) : _t(&t) {}
  constexpr const_reference(const const_reference &) = default;
  constexpr const_reference &operator=(const const_reference &) = default;

 public:
  constexpr operator const T &() const { return *_t; }

  constexpr const T &get() const { return *_t; }
};

template <typename T>
class move_reference {
 private:
  non_owning<T> _t;

 public:
  constexpr ~move_reference() = default;
  constexpr move_reference(T &&t) : _t(&t) {}
  constexpr move_reference(const move_reference &) = default;
  constexpr move_reference &operator=(const move_reference &) = default;

 public:
  constexpr operator T &&() const { return *_t; }

  constexpr T &&get() const { return *_t; }
};

template <typename T>
class universal_reference {
 private:
  T _t;

 public:
  constexpr ~universal_reference() = default;
  constexpr universal_reference() = default;
  constexpr universal_reference(T t) : _t(transfer(t)) {}
  constexpr universal_reference(const universal_reference &) = default;
  constexpr universal_reference &operator=(const universal_reference &) =
      default;

 public:
  constexpr operator T &() { return _t; }

  constexpr T &get() { return _t; }

  constexpr operator const T &() const { return _t; }

  constexpr const T &get() const { return _t; }
};

template <typename T>
class universal_reference<T &&> {
 private:
  move_reference<T> _t;

 public:
  constexpr ~universal_reference() = default;
  constexpr universal_reference(T &&t) : _t(t) {}
  constexpr universal_reference(const universal_reference &) = default;
  constexpr universal_reference &operator=(const universal_reference &) =
      default;

 public:
  constexpr operator T &&() { return _t.get(); }

  constexpr T &&get() { return _t.get(); }

  constexpr operator const T &&() const { return _t.get(); }

  constexpr const T &&get() const { return _t.get(); }
};

template <typename T>
class universal_reference<T &> {
 private:
  reference<T> _t;

 public:
  constexpr ~universal_reference() = default;
  constexpr universal_reference(T &t) : _t(t) {}
  constexpr universal_reference(const universal_reference &) = default;
  constexpr universal_reference &operator=(const universal_reference &) =
      default;

 public:
  constexpr operator T &() { return _t.get(); }

  constexpr T &get() { return _t.get(); }
  constexpr operator const T &() const { return _t.get(); }

  constexpr const T &get() const { return _t.get(); }
};

template <typename T>
class universal_reference<const T &> {
 private:
  const_reference<T> _t;

 public:
  constexpr ~universal_reference() = default;
  constexpr universal_reference(const T &t) : _t(t) {}
  constexpr universal_reference(const universal_reference &) = default;
  constexpr universal_reference &operator=(const universal_reference &) =
      default;

 public:
  constexpr operator const T &() const { return _t.get(); }

  constexpr const T &get() const { return _t.get(); }
};
}  // namespace stew

#endif
