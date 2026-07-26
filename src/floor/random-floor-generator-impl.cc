module randomfloorgenerator;

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../entities/character.cc"
#include "../entities/gold-pile.cc"
#include "../entities/staircase.cc"
#include "../entities/stats.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../enums/room-type.cc"
#include "../enums/tile-type.cc"
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/gold-factory.cc"
#include "cell.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "random-floor-generator.cc"
#include "room.cc"
#else
import <memory>;
import <vector>;
import character;
import dragonhoardfactory;
import enemyfactory;
import goldfactory;
import goldpile;
import goldsize;
import stats;
import staircase;
import racetype;
import tiletype;
import roomtype;
import cell;
import floor;
import chamber;
import position;
import room;
#endif  // __INTELLISENSE__

using namespace std;

RandomFloorGenerator::RandomFloorGenerator(RNG& rng)
    : rng{rng},
      enemyFactory{rng},
      goldFactory{rng},
      potionFactory{rng},
      dragonHoardFactory{rng, enemyFactory} {}

// Selects a random available floor tile and removes it from availableTiles
// TODO: Assumes there are available tiles to select from, should be updated if we do different Floor layout bonus
Position takeRandomAvailableFloorTile(vector<Position>& availableTiles, RNG& rng) {
  std::size_t randomIndex = rng.intRange(availableTiles.size());
  Position selected = availableTiles[randomIndex];

  // Remove the selected tile from the Chamber's list
  availableTiles[randomIndex] = availableTiles.back();
  availableTiles.pop_back();
  return selected;
}

Floor RandomFloorGenerator::generateFloor() {
  // TODO: Maybe use an unordered_set instead of a vector

  Floor floor = createBaseFloor(rng);
  // Get all chambers in the floor
  vector<Chamber*> chambers;
  for (Room* room : floor.getRooms()) {
    if (room->type() == RoomType::Chamber) {
      chambers.push_back(static_cast<Chamber*>(room));
    }
  }

  // Create list of available floor tiles for each chamber
  vector<vector<Position>> availableTiles;
  for (Chamber* chamber : chambers) {
    vector<Position> chamberTiles;
    for (const Cell* cell : chamber->getCells()) {
      if (cell->tileType == TileType::Floor && !cell->isOccupied()) {
        chamberTiles.push_back(cell->position);
      }
    }
    availableTiles.push_back(chamberTiles);
  }
  std::size_t playerChamber = rng.intRange(chambers.size());
  floor.playerSpawn = takeRandomAvailableFloorTile(availableTiles[playerChamber], rng);

  // Staircase must be in a different chamber than the player
  std::size_t staircaseChamber = 0;
  do {
    staircaseChamber = rng.intRange(chambers.size());
  } while (staircaseChamber == playerChamber);

  Position staircasePosition = takeRandomAvailableFloorTile(availableTiles[staircaseChamber], rng);
  floor.getCell(staircasePosition).add(make_shared<Staircase>(staircasePosition));

  for (int i = 0; i < 10; ++i) {
    const int chamberIndex = rng.intRange(static_cast<int>(chambers.size()));
    potionFactory.process(*chambers[chamberIndex], availableTiles[chamberIndex]);
  }

  for (int i = 0; i < RandomFloorGenerator::NUM_GOLD_PILES_SPAWNED; ++i) {
    std::size_t chamberIndex = rng.intRange(chambers.size());
    goldFactory.process(*chambers[chamberIndex], availableTiles[chamberIndex]);
  }

  for (std::size_t i = 0; i < chambers.size(); ++i) {
    dragonHoardFactory.process(*chambers[i], availableTiles[i]);
  }

  for (int i = 0; i < RandomFloorGenerator::NUM_ENEMIES_SPAWNED; ++i) {
    std::size_t chamberIndex = rng.intRange(chambers.size());
    enemyFactory.process(*chambers[chamberIndex], availableTiles[chamberIndex]);
  }

  return floor;
}
