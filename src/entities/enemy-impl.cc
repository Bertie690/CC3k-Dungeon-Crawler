module enemy;

#ifdef __INTELLISENSE__
#include <vector>

#include "../enums/direction.cc"
#include "../enums/tile-type.cc"
#include "../floor/cell.cc"
#include "../floor/room.cc"
#include "enemy.cc"
#else
import <vector>;
import action;
import direction;
import tiletype;
import cell;
import floor;
import room;
#endif  // __INTELLISENSE__

using namespace std;

Action Enemy::getNextMove(Floor& floor) {
  // TODO Dragon override since it doesn't move?
  const Room& room = floor.getRoomAt(position);

  vector<Direction> availableAdjacentCells;
  for (Direction direction : room.getAdjacentCells(position, true)) {
    const Cell& destination = floor.getCell(position + direction);

    // Filters out doors
    if (destination.tileType == TileType::Floor) availableAdjacentCells.push_back(direction);
  }
  if (availableAdjacentCells.empty()) return Pass{};

  int randomIndex = floor.rng.intRange(static_cast<int>(availableAdjacentCells.size()));
  return Move{availableAdjacentCells[randomIndex]};
}
