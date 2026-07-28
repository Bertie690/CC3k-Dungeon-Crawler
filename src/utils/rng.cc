export module rng;

#pragma once

#ifdef __INTELLISENSE__
#include <chrono>
#include <iterator>
#include <limits>
#include <random>
#include <type_traits>
#else
import <random>;
import <limits>;
import <chrono>;
import <iterator>;
import <type_traits>;
#endif  // __INTELLISENSE__

// Concept representing a container with size and random access.
export template <typename T>
concept Container = requires(T c) {
  { c.size() } -> std::integral;
  { c[decltype(c.size()){}] } -> std::convertible_to<const typename T::value_type&>;
} && requires { typename T::value_type; };  // Ensure the container has a value_type defined

// Concept representing a container that can be iterated over to extract a random value.
export template <typename T>
concept IterableContainer = requires(T c) {
  { c.size() } -> std::integral;
  // input iterator access
  { c.begin() } -> std::input_iterator;
  { c.end() } -> std::sentinel_for<decltype(c.begin())>;
  { *c.begin() } -> std::convertible_to<const typename T::value_type&>;
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
  // Works with any signed or unsigned integer type, and throws `std::out_of_range` if the above interval is empty or results in overflow.
  template <typename T>
    requires std::is_integral_v<T>
  T intRange(const T range, const T start = 0);

  // Return a uniformly chosen real number in the interval [0, 1).
  double randDouble();

  // fast O(1) picks when random access exists

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and throws `std::out_of_range` if the container is empty.
  template <Container C>
  typename C::value_type pick(C& c);
  // const overloads needed due to lacking deducing this from C++23

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support random access, and throws `std::out_of_range` if the container is empty.
  template <Container C>
  const typename C::value_type pick(const C& c);

  // Pick a uniformly chosen element from the given known-size array.
  // Throws `std::out_of_range` if the array is empty.
  template <typename T>
    requires std::is_bounded_array_v<T>
  typename std::remove_extent_t<T> pick(T& arr);

  // slower O(n) picks when only input iteration is available

  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support input iteration, and throws `std::out_of_range` if the container is empty.
  template <IterableContainer C>
    requires(!Container<C>)
  typename C::value_type pick(C& c);
  // Pick a uniformly chosen element from the given container.
  // Must have a known size and support input iteration, and throws `std::out_of_range` if the container is empty.
  template <IterableContainer C>
    requires(!Container<C>)
  const typename C::value_type pick(const C& c);
};

#pragma region Implementation
// NB: This all has to be in the same file since the linker throws a temper tantrum if it's separated

RNG::RNG(int seed) : twister(seed) {}

template <typename T>
  requires std::is_integral_v<T>
T RNG::intRange(const T range, const T start) {
  if (range <= 0) {
    throw std::out_of_range("Cannot generate a random integer from an empty range!");
  }
  if (range > std::numeric_limits<T>::max() - start) {
    throw std::overflow_error("Integer range (start " + std::to_string(start) + ", range " +
                              std::to_string(range) + ") would result in integer overflow!");
  }
  // subtract 1 due to inclusivity
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
const typename C::value_type RNG::pick(const C& c) {
  if (c.size() <= 0) {
    throw std::out_of_range("Cannot pick from an empty container!");
  }
  return c[this->intRange(c.size())];
}

template <IterableContainer C>
  requires(!Container<C>)
typename C::value_type RNG::pick(C& c) {
  if (c.size() <= 0) {
    throw std::out_of_range("Cannot pick from an empty container!");
  }
  auto it = c.begin();
  std::advance(it, this->intRange(c.size()));
  return *it;
}
template <IterableContainer C>
  requires(!Container<C>)
const typename C::value_type RNG::pick(const C& c) {
  if (c.size() <= 0) {
    throw std::out_of_range("Cannot pick from an empty container!");
  }
  auto it = c.begin();
  std::advance(it, this->intRange(c.size()));
  return *it;
}

template <typename T>
  requires std::is_bounded_array_v<T>
typename std::remove_extent_t<T> RNG::pick(T& arr) {
  return arr[this->intRange(std::extent_v<T>)];
}
#pragma endregion Implementation
