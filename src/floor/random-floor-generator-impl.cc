module randomfloorgenerator;

#ifdef __INTELLISENSE__
#include <vector>

#include "../enums/tile-type.cc"
#include "cell.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "random-floor-generator.cc"
#else
import <vector>;
import tiletype;
import cell;
import floor;
import chamber;
import position;
#endif  // __INTELLISENSE__

using namespace std;

RandomFloorGenerator::RandomFloorGenerator(RNG& rng) : rng{rng} {}

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
  Floor floor = createBaseFloor(rng);
  // Get all chambers in the floor
  vector<const Room*> chambers;
  for (const Room* room : floor.getRooms()) {
    if (room->type() == RoomType::Chamber) chambers.push_back(room);
  }

  // Create list of available floor tiles for each chamber
  vector<vector<Position>> availableTiles;
  for (const Room* chamber : chambers) {
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

  // TODO: randomly place enemies, potions, gold, stairs
  // Select stairs chamber from non playerChambers
  return floor;
}
