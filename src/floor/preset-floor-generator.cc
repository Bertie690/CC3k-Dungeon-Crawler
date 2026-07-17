export module presetfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include <fstream>
#include <string>
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import <fstream>;
import <string>;
import rng;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class PresetFloorGenerator : public FloorGenerator {
  RNG& rng;
  std::ifstream input;
  int nextFloor = 1;

 public:
 // Generates Floor using file input provided entity placements
  PresetFloorGenerator(RNG& rng, const std::string& fileName);
  Floor generateFloor() override;
};
