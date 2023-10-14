#ifndef __n_properties_hpp__
#define __n_properties_hpp__

#include <core/algorithm.hpp>
#include <core/maybe.hpp>
#include <core/string.hpp>
#include <core/vector.hpp>


  template<iterator I, typename T>
  auto split()
}



template <character C>
struct property {
  string<C> _key;
  string<C> _val;
};

template <character C>
vector<property<C>> parse_properties(const string<C>& _data) {
  vector<property<C>> properties;

 return properties;
}

template <character C>
class properties_reader {};


#endif
