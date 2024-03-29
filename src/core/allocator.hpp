#ifndef __stew_core_allocator_hpp__
#define __stew_core_allocator_hpp__

#include <core/core.hpp>
#include <core/exception.hpp>
#include <new>

template <typename T>
struct allocator {
  constexpr T* allocate(size_t n) const {
    if (n == 0) {
      throw bad_alloc();
    }

    else {
      T* p = static_cast<T*>(::operator new(n * sizeof(T)));

      if (p == nullptr) {
        throw bad_alloc();
      }

      else {
        return p;
      }
    }
  }

  constexpr void deallocate(T* p) const noexcept {
    if (p != nullptr) {
      ::operator delete(p);
    }
  }

  template <typename... A>
  constexpr void construct(T* p, A&&... args) const {
    new (p) T(relay<A>(args)...);
  }

  constexpr void copy(T* dst, const T* src, size_t n) const {
    if (dst != nullptr and src != nullptr and dst != src) {
      for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i];
      }
    }
  }

  constexpr void move(T* dst, const T* src, size_t n) const {
    if (dst != nullptr and src != nullptr and dst != src) {
      for (size_t i = 0; i < n; ++i) {
        dst[i] = move(src[i]);
      }
    }
  }

  constexpr void destroy(T* p) const noexcept {
    if constexpr (not trivial<T>) {
      if (p != nullptr) {
        p->~T();
      }
    }
  }

  constexpr void destroy(T* p, size_t n) const noexcept {
    if constexpr (not trivial<T>) {
      if (p != nullptr) {
        for (size_t i = 0; i < n; ++i) {
          p[i].~T();
        }
      }
    }
  }
};

#endif