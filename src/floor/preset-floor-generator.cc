export module presetfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include <fstream>
#include <string>
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/gold-factory.cc"
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import <fstream>;
import <string>;
import dragonhoardfactory;
import enemyfactory;
import goldfactory;
import rng;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class PresetFloorGenerator : public FloorGenerator {
  RNG& rng;
  EnemyFactory enemyFactory;
  GoldFactory goldFactory;
  DragonHoardFactory dragonHoardFactory;
  std::ifstream input;
  int nextFloor = 1;

 public:
 // Generates Floor using file input provided entity placements
  PresetFloorGenerator(RNG& rng, const std::string& fileName);
  Floor generateFloor() override;
};
