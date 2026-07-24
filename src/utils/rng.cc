export module rng;

#pragma once

#ifdef __INTELLISENSE__
#include <chrono>
#include <random>
#include <type_traits>
#else
import <random>;
import <chrono>;
import <type_traits>;
#endif  // __INTELLISENSE__

// Concept representing a container with size and random access.
export template <typename T>
concept Container = requires(T c) {
  { c.size() } -> std::convertible_to<std::size_t>;
  { c[c.size() - 1] };
} && requires { typename T::value_type; };  // Ensure the container has a value_type defined

// Class representing a seeded random number generator.
export class RNG {
  // The underlying Mersenne Twister engine.
  std::mt19937 twister;

 public:
  // Create a new RNG instance with the given initial seed, using a fallback seed if none is provided.
  //
  // The exact nature of the fallback seed is implementation-defined, but is guaranteed to be
  // different on different runs of the program.
  RNG(int seed = std::chrono::system_clock::now().time_since_epoch().count());

  // Generate a uniformly chosen integer in the interval [start, start + range).
  int intRange(int range, int start = 0);

  // Return a uniformly chosen real number in the interval [0, 1).
  double randDouble();

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and throws `std::out_of_range` if the container is empty.
  template <Container C>
  decltype(auto) pick(C& c);

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and returns the specified default value if the array is empty
  // (all other errors will still propagate upwards).
  template <Container C>
  decltype(auto) pick(C& c, const typename C::value_type& defaultValue);

  // Pick a uniformly chosen element from the given known-size array.
  // Throws `std::out_of_range` if the array is empty.
  template <typename T>
    requires std::is_bounded_array_v<T>
  decltype(auto) pick(T& arr);
};
