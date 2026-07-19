module enemy;

#ifdef __INTELLISENSE__
#include <vector>

#include "../enums/direction.cc"
#include "../floor/room.cc"
#include "enemy.cc"
#else
import <vector>;
import action;
import direction;
import floor;
import room;
#endif  // __INTELLISENSE__

using namespace std;

Action Enemy::getNextMove(Floor& floor) {
  // TODO Dragon override since it doesn't move?
  const Room& room = floor.getRoomAt(position);
  vector<Direction> availableAdjacentCells = room.getAdjacentCells(position, true);

  if (availableAdjacentCells.empty()) return Pass{};

  int randomIndex = floor.rng.intRange(static_cast<int>(availableAdjacentCells.size()));
  return Move{availableAdjacentCells[randomIndex]};
}
