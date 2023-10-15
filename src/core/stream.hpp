#ifndef __steward_core_stream_hpp__
#define __steward_core_stream_hpp__

#include <core/collection.hpp>
#include <core/utils.hpp>
/**
 * @file
 * @brief This file defines concepts related to streams.
 *
 * In this file, you will find concepts that describe how streams work.
 *
 * An input stream is a way to iterate through content with only two essential
 * functions:
 * - `has()`: A function that indicates whether there is more data to read in
 * the content.
 * - `next()`: A function that provides the next element from the content.
 *
 * It's important to note that calling `next()` consumes the value, meaning that
 * it cannot be retrieved again from the same input stream unless you create a
 * new instance of the input stream.
 *
 * On the other hand, an output stream is a concept where you have a single
 * critical function:
 * - `add(value)`: This function allows you to append a value to the content
 * behind the output stream.
 */

/**
 * @brief Generic input concept for streams.
 *
 * This concept checks if a given type, I, meets the requirements of a generic
 * input stream. To satisfy this concept, type I must have a 'type' type
 * definition and two functions:
 * - i.next(): which returns a value of the same type as 'typename I::type'.
 * - i.has(): which returns a boolean indicating whether the input stream still
 * has data.
 */
template <typename I>
concept input_stream = requires { typename I::type; } and requires(I i) {
  { i.next() } -> same_as_declined<typename I::type>;
  { i.has() } -> same_as<bool>;
};

/**
 * @brief Generic output concept for streams.
 *
 * This concept checks if a given type, O, meets the requirements of a generic
 * output stream. To satisfy this concept, type O must have a 'type' type
 * definition and a function:
 * - o.add(c): which takes an argument of the same type as 'typename O::type'
 * and returns a boolean indicating the success of the addition.
 */
template <typename O>
concept output_stream =
    requires { typename O::type; } and requires(O o, typename O::type c) {
      { o.add(c) } -> same_as<bool>;
    };

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