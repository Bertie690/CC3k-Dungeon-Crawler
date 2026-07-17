export module floorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include "../utils/rng.cc"
#include "floor.cc"
#else
import rng;
import floor;
#endif  // __INTELLISENSE__

export class FloorGenerator {
 protected:
  // Creates a Floor containing the fixed Chambers and Passage layout
  static Floor createBaseFloor(RNG& rng);

 public:
  virtual Floor generateFloor() = 0;
  virtual ~FloorGenerator() = default;
};
