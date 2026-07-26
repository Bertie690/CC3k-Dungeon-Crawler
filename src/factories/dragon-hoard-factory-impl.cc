module dragonhoardfactory;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include <vector>

#include "../entities/dragon-hoard.cc"
#include "../entities/enemy.cc"
#include "../enums/direction.cc"
#include "dragon-hoard-factory.cc"
#else
import <algorithm>;
import <memory>;
import <vector>;
import direction;
import dragonhoard;
import enemy;
#endif  // __INTELLISENSE__

using namespace std;

DragonHoardFactory::DragonHoardFactory(RNG& rng, EnemyFactory& enemyFactory)
    : FactoryBase{rng}, enemyFactory{enemyFactory} {}

// Find the position in availablePositions and remove it
void removeAvailablePosition(vector<Position>& availablePositions, const Position& position) {
  vector<Position>::iterator positionIter = availablePositions.begin();
  while (*positionIter != position) {
    ++positionIter;
  }
  *positionIter = availablePositions.back();
  availablePositions.pop_back();
}

void DragonHoardFactory::place(Room& room, const Position& hoardPosition,
                               const Position& dragonPosition) {
  shared_ptr<Enemy> dragon = enemyFactory.createDragon(dragonPosition, hoardPosition);
  shared_ptr<DragonHoard> hoard = make_shared<DragonHoard>(hoardPosition, dragon);
  room[hoardPosition].add(hoard);
  room[dragonPosition].add(dragon);
}

void DragonHoardFactory::process(Room& room, vector<Position>& availablePositions) {
  while (true) {
    Position hoardPosition = rng.pick(availablePositions);

    vector<Position> validDragonPositions;
    for (const Direction direction : room.getAdjacentCells(hoardPosition, true)) {
      Position pos = hoardPosition + direction;
      // Check which adjacent cells are available to place a Dragon
      if (find(availablePositions.begin(), availablePositions.end(), pos) !=
          availablePositions.end()) {
        validDragonPositions.push_back(pos);
      }
    }
    if (validDragonPositions.empty())
      continue;  // select another hoard position that isn't blocked off

    Position dragonPosition = rng.pick(validDragonPositions);
    removeAvailablePosition(availablePositions, hoardPosition);
    removeAvailablePosition(availablePositions, dragonPosition);
    place(room, hoardPosition, dragonPosition);
    return;
  }
}

void DragonHoardFactory::process(Room& room, const Position& hoardPosition,
                                 const Position& dragonPosition) {
  place(room, hoardPosition, dragonPosition);
}
