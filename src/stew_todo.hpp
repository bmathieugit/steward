#ifndef __stew_todo_hpp__
#define __stew_todo_hpp__

namespace stew
{
  template <typename T0, typename... Tn>
  consteval size_t sizeofmax()
  {
    if constexpr (sizeof...(Tn) == 0)
    {
      return sizeof(T0);
    }
    else
    {
      if (sizeof(T0) > sizeofmax<Tn...>())
      {
        return sizeof(T0);
      }
      else
      {
        return sizeofmax<Tn...>();
      }
    }
  }

  namespace impl
  {
    template <size_t I, typename H, typename... T>
    struct indexof
    {
      static_assert(sizeof...(T) > 0, "indexof : search type not in types pack");
    };

    template <size_t I, typename H, typename T0, typename... Tn>
    struct indexof<I, H, T0, Tn...>
    {
      static constexpr size_t value = indexof<I + 1, H, Tn...>::value;
    };

    template <size_t I, typename T, typename... Tn>
    struct indexof<I, T, T, Tn...>
    {
      static constexpr size_t value = I;
    };
  }

  template <typename H, typename... T>
  constexpr size_t indexof = impl::indexof<0, H, T...>::value;

  namespace impl
  {
    template <size_t I, typename T0, typename... Tn>
    struct typeat : struct_type<typeat<I - 1, Tn...>>
    {
    };

    template <typename T0, typename... Tn>
    struct typeat<0, T0, Tn...> : struct_type<T0>
    {
    };
  }

  template <size_t I, typename... T>
  using typeat = type<impl::typeat<I, T...>>;

  namespace todo
  {
    template <class T>
    struct type_identity
    {
      using type = T;
    };

    template <typename T>
    using type_identity_t = typename type_identity<T>::type;

    template <size_t... I>
    struct isequence
    {
    };

    namespace impl
    {
      template <size_t I0, size_t... In>
      consteval auto make_isequence()
      {
        if constexpr (I0 == 0)
        {
          return isequence<I0, In...>{};
        }
        else
        {
          return make_isequence<I0 - 1, I0, In...>();
        }
      }
    }

    template <size_t N>
      requires(N > 0)
    constexpr auto make_isequence()
    {
      return impl::make_isequence<N - 1>();
    }

    template <typename T>
    constexpr size_t array_size = 0;

    template <typename... T>
    constexpr size_t array_size<tuple<T...>> = sizeof...(T);

    template <typename T, typename F, size_t... I>
    consteval auto apply(T &&t, F &&f, isequence<I...>) -> decltype(auto)
    {
      return relay<F>(f)(get<I>(relay<T>(t))...);
    }

    template <typename T, typename F>
    consteval auto apply(T &&t, F &&f) -> decltype(auto)
    {
      return apply(relay<T>(t), relay<F>(f), make_isequence<array_size<rm_cvref<T>>>());
    }
  }

  namespace todo
  {
    template <typename T>
    class list
    {
      struct node
      {
        T _t;
        size_t _next = static_cast<size_t>(-1);

      public:
        ~node() = default;
        node() = default;
        template <convertible_to<T> U>
        node(U &&u) : _t(relay<U>(u)) {}
        node(const node &o) : _t(o._t), _next(o._next) {}
        node(node &&o) : _t(transfer(o._t)), _next(transfer(o._next))
        {
          o._next = static_cast<size_t>(-1);
        }

        node &operator=(node o)
        {
          _t = transfer(o._t);
          _next = transfer(o._next);
          return *this;
        }
      };

      struct iterator
      {
        non_owning<list> _l;
        size_t _current = static_cast<size_t>(-1);

        iterator &operator++()
        {
          if (_current != static_cast<size_t>(-1))
          {
            _current = _l->_nodes[_current]._next;
          }

          return *this;
        }

        iterator operator++(int)
        {
          iterator copy = *this;
          ++this;
          return copy;
        }

        bool operator==(const iterator &o) const
        {
          return _l == o._l && _current == o._current;
        }

        bool operator!=(const iterator &o) const
        {
          return !(*this == o);
        }

        T &operator*() const
        {
          return _l->_nodes[_current]._t;
        }
      };

      struct const_iterator
      {
        non_owning<const list> _l;
        size_t _current = static_cast<size_t>(-1);

        const_iterator &operator++()
        {
          if (_current != static_cast<size_t>(-1))
          {
            _current = _l->_nodes[_current]._next;
          }

          return *this;
        }

        const_iterator operator++(int)
        {
          iterator copy = *this;
          ++this;
          return copy;
        }

        bool operator==(const const_iterator &o) const
        {
          return _l == o._l && _current == o._current;
        }

        bool operator!=(const const_iterator &o) const
        {
          return !(*this == o);
        }

        const T &operator*() const
        {
          return _l->_nodes[_current]._t;
        }
      };

    private:
      vector<node> _nodes;
      size_t _first = static_cast<size_t>(-1);
      size_t _last = static_cast<size_t>(-1);

    public:
      ~list() = default;
      list() = default;
      list(const list &o) = default;
      list &operator=(const list &) = default;
      list &operator=(list &&) = default;

    public:
      template <convertible_to<T> U>
      void push(U &&u)
      {
        _nodes.push(node(relay<U>(u)));

        if (_last == static_cast<size_t>(-1))
        {
          _first = 0;
          _last = 0;
        }
        else
        {
          _nodes[_last]._next = _nodes.size() - 1;
          _last = _nodes.size() - 1;
        }
      }

      template <range R>
      void push(R &&r)
      {
        for (auto &&i : relay<R>(r))
        {
          push(relay<decltype(i)>(i));
        }
      }

      template <convertible_to<T> U>
      void push_front(U &&u)
      {
        _nodes.push(node(relay<U>(u)));

        if (_first == static_cast<size_t>(-1))
        {
          _first = 0;
          _last = 0;
        }
        else
        {
          _nodes[_nodes.size() - 1]._next = _first;
          _first = _nodes.size() - 1;
        }
      }

      template <range R>
      void push_front(R &&r)
      {
        for (auto &&i : relay<R>(r))
        {
          push_front(relay<decltype(i)>(i));
        }
      }

      template <typename I, convertible_to<T> U>
        requires same_one_of<I, iterator, const_iterator>
      void insert(I i, U &&u)
      {
        // TODO: faire la fonction insert
      }

    public:
      auto size() const
      {
        return _nodes.size();
      }

      bool empty() const
      {
        return _nodes.empty();
      }

    public:
      auto begin()
      {
        return iterator{this, _first};
      }

      auto end()
      {
        return iterator{this, static_cast<size_t>(-1)};
      }

      auto begin() const
      {
        return const_iterator{this, _first};
      }

      auto end() const
      {
        return const_iterator{this, static_cast<size_t>(-1)};
      }
    };

    template <typename T>
    class set
    {
    private:
      vector<T> _data;

    public:
      ~set() = default;
      set() = default;
      set(const set &) = default;
      set(set &&) = default;
      set &operator=(const set &) = default;
      set &operator=(set &&) = default;

    public:
      auto size() const
      {
        return _data.size();
      }

      auto empty() const
      {
        return _data.empty();
      }

    public:
      auto begin()
      {
        return _data.begin();
      }

      auto end()
      {
        return _data.end();
      }

      auto begin() const
      {
        return _data.begin();
      }

      auto end() const
      {
        return _data.end();
      }

    public:
      template <convertible_to<T> U>
      void push(U &&u)
      {
        if (find(_data, relay<U>(u)) == _data.end())
        {
          _data.push(relay<U>(u));
        }
      }

      template <convertible_to<T> U>
      void pop(const U &u)
      {
      }
    };
  }

}

#endif