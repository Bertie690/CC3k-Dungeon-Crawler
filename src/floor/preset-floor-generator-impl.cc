module presetfloorgenerator;

#ifdef __INTELLISENSE__
#include <fstream>
#include <string>
#include <vector>

#include "../entities/staircase.cc"
#include "floor.cc"
#include "preset-floor-generator.cc"
#else
import <fstream>;
import <string>;
import <vector>;
import floor;
import staircase;
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

void placePresetEntities(Floor& floor, const vector<string>& floorLines) {
  for (int y = 0; y < Floor::HEIGHT; ++y) {
    for (int x = 0; x < Floor::WIDTH; ++x) {
      const char c = floorLines[y][x];
      const Position position{x, y};
      switch (c) {
        case '\\':
          floor.getCell(position).add(make_shared<Staircase>(position));
          break;
        case '@':
          floor.playerSpawn = position;
          break;
        case 'H':
        case 'W':
        case 'E':
        case 'O':
        case 'M':
        case 'D':
        case 'L':
          // TODO: create enemy: EnemyFactory
          break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
          // TODO: create potion: PotionFactory
          break;
        case '6':
        case '7':
        case '8':
        case '9':
          // TODO: create gold : GoldFactory
          break;
      }
    }
  }
}

Floor PresetFloorGenerator::generateFloor() {
  vector<string> floorLines = readFloorLines(input);
  ++nextFloor;

  Floor floor = createBaseFloor(rng);

  // TODO: add factories to signature
  placePresetEntities(floor, floorLines);
  return floor;
}
