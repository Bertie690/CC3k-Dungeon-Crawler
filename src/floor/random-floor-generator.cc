export module randomfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import rng;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class RandomFloorGenerator : public FloorGenerator {
  RNG& rng;

 public:
  RandomFloorGenerator(RNG& rng);
  // Generates Floor using random entity placements
  Floor generateFloor() override;
};
