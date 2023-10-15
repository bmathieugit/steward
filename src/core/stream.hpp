#ifndef __steward_core_stream_hpp__
#define __steward_core_stream_hpp__

#include <core/collection.hpp>
#include <core/utils.hpp>




/**
 * @brief Input Stream based on iterators.
 *
 * This class defines an input stream that operates based on iterators, allowing
 * you to iterate through a sequence of elements using the provided iterator.
 *
 * @tparam I The iterator type to be used with this input stream.
 */
template <iterator I>
class iterator_input_stream {
 public:
  using type = typename I::type; /**< The type of elements in the stream. */

 private:
  I _i; /**< The underlying iterator. */

 public:
  /**
   * @brief Constructor for the iterator_input_stream.
   *
   * @param i The iterator to initialize the input stream.
   */
  constexpr iterator_input_stream(I i) : _i(i) {}

 public:
  /**
   * @brief Check if there are more elements in the stream.
   *
   * This function checks whether there are more elements available for reading
   * in the stream.
   *
   * @return True if there are more elements to read, otherwise false.
   */
  constexpr auto has() const { return _i.has(); }

  /**
   * @brief Get the next element from the stream.
   *
   * This function retrieves the next element from the stream and advances the
   * internal iterator to the next element. Calling this function consumes the
   * value, meaning it cannot be retrieved again from the same input stream
   * instance.
   *
   * @return The next element from the stream.
   */
  constexpr auto next() -> decltype(auto) {
    auto&& item = _i.get();
    _i.next();
    return relay<decltype(item)>(item);
  }
};

#endif