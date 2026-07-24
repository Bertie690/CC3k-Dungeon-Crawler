module presetfloorgenerator;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../entities/staircase.cc"
#include "../enums/direction.cc"
#include "../enums/gold-size.cc"
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/gold-factory.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "preset-floor-generator.cc"
#include "room.cc"
#else
import <algorithm>;
import <fstream>;
import <string>;
import <vector>;
import <memory>;
import <stdexcept>;
import direction;
import dragonhoardfactory;
import goldsize;
import enemyfactory;
import goldfactory;
import chamber;
import position;
import floor;
import room;
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
Position findAdjacentHoard(const Floor& floor, const vector<string>& floorLines, const Position& dragonPosition, const vector<Position>& claimedDragonHoards) {
  const Room& chamber = floor.getRoomAt(dragonPosition);
  for (const Direction direction : chamber.getAdjacentCells(dragonPosition)) {
    Position candidate = dragonPosition + direction;
    // Check if candidate is a Dragon Hoard and not already claimed
    if (floorLines[candidate.y][candidate.x] == '9' && find(claimedDragonHoards.begin(), claimedDragonHoards.end(), candidate) == claimedDragonHoards.end()) {
      return candidate;
    }
  }
  throw invalid_argument{"No adjacent Dragon Hoard found"};
}

PresetFloorGenerator::PresetFloorGenerator(RNG& rng, const string& fileName) : rng{rng}, enemyFactory{rng}, goldFactory{rng}, dragonHoardFactory{rng, enemyFactory}, input{fileName} {}

void placePresetEntities(Floor& floor, const vector<string>& floorLines, GoldFactory& goldFactory, EnemyFactory& enemyFactory, DragonHoardFactory& dragonHoardFactory) {
  vector<Position> claimedDragonHoards;

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
        case 'L':
          floor.getCell(position).add(enemyFactory.create(position, c));
          break;
        case 'D': {
          Position hoardPosition = findAdjacentHoard(floor, floorLines, position, claimedDragonHoards);
          claimedDragonHoards.push_back(hoardPosition);
          dragonHoardFactory.process(static_cast<Chamber&>(floor.getRoomAt(hoardPosition)), hoardPosition, position);
          break;
        }
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
          // TODO: create potion: PotionFactory
          break;
        case '6':
          floor.getCell(position).add(goldFactory.create(position, GoldSize::Normal));
          break;
        case '7':
          floor.getCell(position).add(goldFactory.create(position, GoldSize::Small));
          break;
        case '8':
          floor.getCell(position).add(goldFactory.create(position, GoldSize::MerchantHoard));
          break;
        case '9':
          // Gets placed by dragonHoardFactory
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
  placePresetEntities(floor, floorLines, goldFactory, enemyFactory, dragonHoardFactory);
  return floor;
}
