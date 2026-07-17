module presetfloorgenerator;

#ifdef __INTELLISENSE__
#include <fstream>
#include <string>
#include <vector>
#include "floor.cc"
#include "preset-floor-generator.cc"
#else
import <fstream>;
import <string>;
import <vector>;
import floor;
#endif  // __INTELLISENSE__

using namespace std;

vector<string> readFloorLines(ifstream& input) {
  vector<string> floorLines;
  for (int y = 0; y < Floor::HEIGHT; ++y) {
    // TODO: invalid input handling necessary? Probably not..
    floorLines.emplace_back();
    getline(input, floorLines.back());
  }
  return floorLines;
}

PresetFloorGenerator::PresetFloorGenerator(RNG& rng, const string& fileName) : rng{rng}, input{fileName} {}

Floor PresetFloorGenerator::generateFloor() {
  vector<string> floorLines = readFloorLines(input);
  ++nextFloor;

  Floor floor = createBaseFloor(rng);

  // TODO: place @, enemies, potions, gold, stairs
  return floor;
}
