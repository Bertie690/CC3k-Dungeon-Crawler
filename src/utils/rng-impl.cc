module rng;

#ifdef __INTELLISENSE__
#include <chrono>
#include <random>
#include <type_traits>

#include "rng.cc"
#endif  // __INTELLISENSE__

RNG::RNG(int seed) : twister(seed) {}

int RNG::intRange(int range, int start) {
  std::uniform_int_distribution dist(start, start + range - 1);
  return dist(twister);
}

double RNG::randDouble() {
  std::uniform_real_distribution dist(0.0, 1.0);
  return dist(twister);
}

template <Container C>
typename C::value_type& RNG::pick(C& c) {
  if (c.size() <= 0) {
    throw std::out_of_range("Cannot pick from an empty container!");
  }
  std::uniform_int_distribution dist(0, c.size() - 1);
  return c[dist(twister)];
}

template <typename T>
  requires std::is_bounded_array_v<T>
typename std::remove_extent_t<T>& RNG::pick(T& arr) {
  std::uniform_int_distribution dist(0, std::extent_v<T> - 1);
  return arr[dist(twister)];
}
