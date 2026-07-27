module presetfloorgenerator;

#ifdef __INTELLISENSE__
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../entities/staircase.cc"
#include "../enums/direction.cc"
#include "../enums/gold-size.cc"
#include "../enums/potion-type.cc"
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/gold-factory.cc"
#include "../factories/potion-factory.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "preset-floor-generator.cc"
#include "room.cc"
#else
import <fstream>;
import <string>;
import <vector>;
import <memory>;
import <stdexcept>;
import direction;
import dragonhoardfactory;
import goldsize;
import potiontype;
import enemyfactory;
import goldfactory;
import potionfactory;
import chamber;
import position;
import floor;
import room;
import staircase;
#endif  // __INTELLISENSE__

using namespace std;
using namespace PresetSymbols;

vector<string> readFloorLines(ifstream& input) {
  vector<string> floorLines;
  for (int y = 0; y < Floor::HEIGHT; ++y) {
    // TODO: invalid input handling necessary? Probably not..
    if (input.eof()) {
      throw invalid_argument{"Unexpected end of file while reading floor layout!"};
    }
    floorLines.emplace_back();
    getline(input, floorLines.back());
  }
  return floorLines;
}

PresetFloorGenerator::PresetFloorGenerator(RNG& rng, const string& fileName,
                                           unsigned int floorNumber,
                                           bool useImprovedHoardResolution)
    : rng{rng},
      enemyFactory{rng},
      goldFactory{rng},
      potionFactory{rng},
      nextFloor{floorNumber + 1U},
      dragonHoardFactory{rng, enemyFactory},
      strategy{useImprovedHoardResolution ? static_cast<std::unique_ptr<HoardPlacementStrategy>>(
                                                make_unique<PerfectMatchHoardPlacementStrategy>(
                                                    dragonHoardFactory, goldFactory))
                                          : static_cast<std::unique_ptr<HoardPlacementStrategy>>(
                                                make_unique<DefaultHoardPlacementStrategy>(
                                                    dragonHoardFactory, goldFactory))},
      input{fileName} {}

void PresetFloorGenerator::placePresetEntities(Floor& floor, const vector<string>& floorLines) {
  vector<Position> dragonHoards;
  vector<Position> dragons;

  for (int y = 0; y < Floor::HEIGHT; ++y) {
    for (int x = 0; x < Floor::WIDTH; ++x) {
      const char c = floorLines[y][x];
      const Position position{x, y};
      switch (c) {
        case STAIRCASE_SYMBOL:
          floor.getCell(position).add(make_shared<Staircase>(position));
          break;
        case PLAYER_SPAWN_SYMBOL:
          floor.playerSpawn = position;
          break;
        case HUMAN_SYMBOL:
        case DWARF_SYMBOL:
        case ELF_SYMBOL:
        case ORC_SYMBOL:
        case MERCHANT_SYMBOL:
        case HALFLING_SYMBOL:
          floor.getCell(position).add(enemyFactory.create(position, c));
          break;
        case RH_POTION_SYMBOL:
        case BA_POTION_SYMBOL:
        case BD_POTION_SYMBOL:
        case PH_POTION_SYMBOL:
        case WA_POTION_SYMBOL:
        case WD_POTION_SYMBOL:
          floor.getCell(position).add(
              potionFactory.create(position, static_cast<PotionType>(c - '0')));
          break;
        case NORMAL_GOLD_SYMBOL:
          floor.getCell(position).add(goldFactory.create(position, GoldSize::Normal));
          break;
        case SMALL_HOARD_SYMBOL:
          floor.getCell(position).add(goldFactory.create(position, GoldSize::Small));
          break;
        case MERCHANT_HOARD_SYMBOL:
          floor.getCell(position).add(goldFactory.create(position, GoldSize::MerchantHoard));
          break;
        // Get placed later on
        case DRAGON_SYMBOL:
          dragons.push_back(position);
          break;
        case DRAGON_HOARD_SYMBOL:
          dragonHoards.push_back(position);
          break;
      }
    }
  }

  this->strategy->placeDragonHoards(floor, dragonHoards, dragons);
}

Floor PresetFloorGenerator::generateFloor() {
  vector<string> floorLines = readFloorLines(input);
  ++nextFloor;

  Floor floor = createBaseFloor(rng);

  placePresetEntities(floor, floorLines);
  return floor;
}
