export module rng;

#ifdef __INTELLISENSE__
#include <random>
#include <chrono>
#else
import <random>;
import <chrono>;
#endif // __INTELLISENSE__

// Class representing a seeded random number generator.
class RNG {
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
};