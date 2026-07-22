export module randomfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include "../factories/gold-factory.cc"
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import goldfactory;
import rng;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class RandomFloorGenerator : public FloorGenerator {
  RNG& rng;
  GoldFactory goldFactory;

 public:
  RandomFloorGenerator(RNG& rng);
  // Generates Floor using random entity placements
  Floor generateFloor() override;
};
