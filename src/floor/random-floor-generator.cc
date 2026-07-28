export module randomfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../factories/factory-base.cc"
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import <memory>;
import <vector>;
import factorybase;
import rng;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class RandomFloorGenerator : public FloorGenerator {
  static inline const int NUM_POTIONS_SPAWNED = 10;
  static inline const int NUM_GOLD_PILES_SPAWNED = 10;
  static inline const int NUM_ENEMIES_SPAWNED = 20;

  RNG& rng;
  // An ordered vector of all factories used to generate entities on the floor.
  // They will be applied in the order they are listed.
  std::vector<std::shared_ptr<FactoryBase>> factories;

  std::vector<std::shared_ptr<FactoryBase>> initializeFactories();

 public:
  RandomFloorGenerator(RNG& rng);
  // Generate a Floor using random entity placements.
  virtual Floor generateFloor() override;
};
