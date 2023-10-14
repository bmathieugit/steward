#ifndef __n_ostream_hpp__
#define __n_ostream_hpp__

#include <core/utils.hpp>



template <typename O>
concept ostream =
    requires { typename O::type; } and requires(O o, typename O::type c) {
      { o.add(c) } -> same_as<bool>;
    };



#endif
