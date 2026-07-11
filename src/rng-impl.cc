module rng;

#ifdef __INTELLISENSE__
#include <chrono>
#include <random>

#include "rng.cc"
#endif  // __INTELLISENSE__

RNG::RNG(int seed) : twister(seed) {}

int RNG::intRange(int range, int start) {
  std::uniform_int_distribution dist(start, start + range - 1);
  return dist(twister);
}