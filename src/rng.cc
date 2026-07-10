export module rng;

#ifdef __INTELLISENSE__
#include <random>
#include <chrono>
#else
import <random>;
import <chrono>;
#endif // __INTELLISENSE__

class RNG {
  std::mt19937 twister;

 public:
  RNG() : twister{std::chrono::system_clock::now().time_since_epoch().count()} {}
};