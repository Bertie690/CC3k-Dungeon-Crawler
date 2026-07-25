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
  { c.size() } -> std::integral;
  { c[decltype(c.size()){}] } -> std::same_as<typename T::value_type&>;
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
  template <typename T>
    requires std::is_integral_v<T>
  T intRange(T range, T start = 0);

  // Return a uniformly chosen real number in the interval [0, 1).
  double randDouble();

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and throws `std::out_of_range` if the container is empty.
  template <Container C>
  typename C::value_type pick(C& c);

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and returns the specified default value if the array is empty
  // (all other errors will still propagate upwards).
  template <Container C>
  typename C::value_type pick(C& c, const typename C::value_type& defaultValue);

  // const overloads needed due to lacking deducing this from C++23

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and throws `std::out_of_range` if the container is empty.
  template <Container C>
  const typename C::value_type pick(const C& c);
  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and returns the specified default value if the array is empty
  // (all other errors will still propagate upwards).
  template <Container C>
  const typename C::value_type pick(const C& c, const typename C::value_type& defaultValue);

  // Pick a uniformly chosen element from the given known-size array.
  // Throws `std::out_of_range` if the array is empty.
  template <typename T>
    requires std::is_bounded_array_v<T>
  typename std::remove_extent_t<T> pick(T& arr);
};

#pragma region Implementation
RNG::RNG(int seed) : twister(seed) {}

template <typename T>
  requires std::is_integral_v<T>
T RNG::intRange(T range, T start) {
  std::uniform_int_distribution<T> dist(start, start + range - 1);
  return dist(twister);
}

double RNG::randDouble() {
  std::uniform_real_distribution dist(0.0, 1.0);
  return dist(twister);
}

template <Container C>
typename C::value_type RNG::pick(C& c) {
  if (c.size() <= 0) {
    throw std::out_of_range("Cannot pick from an empty container!");
  }
  return c[this->intRange(c.size())];
}
template <Container C>
typename C::value_type RNG::pick(C& c, const typename C::value_type& defaultValue) {
  try {
    return this->pick(c);
  } catch (const std::out_of_range&) {
    return defaultValue;
  }
}
template <Container C>
const typename C::value_type RNG::pick(const C& c) {
  if (c.size() <= 0) {
    throw std::out_of_range("Cannot pick from an empty container!");
  }
  return c[this->intRange(c.size())];
}
template <Container C>
const typename C::value_type RNG::pick(const C& c, const typename C::value_type& defaultValue) {
  try {
    return this->pick(c);
  } catch (const std::out_of_range&) {
    return defaultValue;
  }
}

template <typename T>
  requires std::is_bounded_array_v<T>
typename std::remove_extent_t<T> RNG::pick(T& arr) {
  return arr[this->intRange(std::extent_v<T>)];
}
#pragma endregion Implementation
