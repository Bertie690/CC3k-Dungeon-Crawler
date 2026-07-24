export module randomfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/gold-factory.cc"
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import dragonhoardfactory;
import enemyfactory;
import goldfactory;
import rng;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class RandomFloorGenerator : public FloorGenerator {
  RNG& rng;
  EnemyFactory enemyFactory;
  GoldFactory goldFactory;
  DragonHoardFactory dragonHoardFactory;

 public:
  RandomFloorGenerator(RNG& rng);
  // Generates Floor using random entity placements
  Floor generateFloor() override;
};
