export module factorybase;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>

#include "../floor/chamber.cc"
#include "../floor/position.cc"
#include "../utils/rng.cc"
#else
import <vector>;
import chamber;
import position;
import rng;
#endif  // __INTELLISENSE__

// FactoryBase is the base class for all entity factories.
export class FactoryBase {
 protected:
  RNG& rng;

 public:
  FactoryBase(RNG& rng) : rng{rng} {}
  virtual ~FactoryBase() = default;

  // Create an Entity within the Chamber using the reserved list of avalablePositions.
  virtual void process(const Chamber& chamber,
                       std::vector<Position>& availablePositions) = 0;
};
