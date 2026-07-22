module randomfloorgenerator;

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../entities/enemy.cc"
#include "../entities/character.cc"
#include "../entities/gold-pile.cc"
#include "../entities/staircase.cc"
#include "../entities/stats.cc"
#include "../factories/gold-factory.cc"
#include "../enums/race-type.cc"
#include "../enums/room-type.cc"
#include "../enums/tile-type.cc"
#include "cell.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "random-floor-generator.cc"
#include "room.cc"
#else
import <memory>;
import <vector>;
import enemy;
import character;
import goldfactory;
import goldpile;
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

RandomFloorGenerator::RandomFloorGenerator(RNG& rng) : rng{rng}, goldFactory{rng} {}

// Selects a random available floor tile and removes it from availableTiles
// TODO: Assumes there are available tiles to select from, should be updated if we do different Floor layout bonus
Position takeRandomAvailableFloorTile(vector<Position>& availableTiles, RNG& rng) {
  int randomIndex = rng.intRange(static_cast<int>(availableTiles.size()));
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
  vector<const Chamber*> chambers;
  for (const Room* room : floor.getRooms()) {
    if (room->type() == RoomType::Chamber) {
      chambers.push_back(static_cast<const Chamber*>(room));
    }
  }

  // Create list of available floor tiles for each chamber
  vector<vector<Position>> availableTiles;
  for (const Chamber* chamber : chambers) {
    vector<Position> chamberTiles;
    for (const Cell* cell : chamber->getCells()) {
      if (cell->tileType == TileType::Floor && !cell->isOccupied()) {
        chamberTiles.push_back(cell->position);
      }
    }
    availableTiles.push_back(chamberTiles);
  }
  int playerChamber = rng.intRange(static_cast<int>(chambers.size()));
  floor.playerSpawn = takeRandomAvailableFloorTile(availableTiles[playerChamber], rng);

  // Staircase must be in a different chamber than the player
  int staircaseChamber = 0;
  do {
    staircaseChamber = rng.intRange(static_cast<int>(chambers.size()));
  } while (staircaseChamber == playerChamber);

  Position staircasePosition = takeRandomAvailableFloorTile(availableTiles[staircaseChamber], rng);
  floor.getCell(staircasePosition).add(make_shared<Staircase>(staircasePosition));

  // TODO: spawn potions

  for (int i = 0; i < 10; ++i) {
    int chamberIndex = rng.intRange(static_cast<int>(chambers.size()));
    goldFactory.process(*chambers[chamberIndex], availableTiles[chamberIndex]);
  }

  // TEMP Demo: Spawn 20 Human enemies until enemy factories are made
  // TODO remove + add real enemy spawn
  for (int i = 0; i < 20; ++i) {
    int chamberIndex = rng.intRange(static_cast<int>(chambers.size()));
    Position position = takeRandomAvailableFloorTile(availableTiles[chamberIndex], rng);
    floor.getCell(position).add(make_shared<Enemy>(position, Stats{140, 20, 20}, RaceType::Human,
                                                   CharacterMoveStrategyType::Random));
  }

  return floor;
}
