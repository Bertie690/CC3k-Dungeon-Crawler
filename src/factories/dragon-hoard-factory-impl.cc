module dragonhoardfactory;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include "../entities/dragon-hoard.cc"
#include "../entities/enemy.cc"
#include <vector>
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

DragonHoardFactory::DragonHoardFactory(RNG& rng, EnemyFactory& enemyFactory) : FactoryBase{rng}, enemyFactory{enemyFactory} {}

// Find the position in availablePositions and remove it
void removeAvailablePosition(vector<Position>& availablePositions, const Position& position) {
  vector<Position>::iterator positionIter = availablePositions.begin();
  while (*positionIter != position) {++positionIter;}
  *positionIter = availablePositions.back();
  availablePositions.pop_back();
}

void DragonHoardFactory::place(Chamber& chamber, const Position& hoardPosition, const Position& dragonPosition) {
  shared_ptr<Enemy> dragon = enemyFactory.createDragon(dragonPosition, hoardPosition);
  shared_ptr<DragonHoard> hoard = make_shared<DragonHoard>(hoardPosition, dragon);
  chamber[hoardPosition].add(hoard);
  chamber[dragonPosition].add(dragon);
}

void DragonHoardFactory::process(Chamber& chamber, vector<Position>& availablePositions) {
  while (true) {
    Position hoardPosition = availablePositions[rng.intRange(static_cast<int>(availablePositions.size()))];
    vector<Position> validDragonPositions;
    for (const Direction direction : chamber.getAdjacentCells(hoardPosition, true)) {
      Position pos = hoardPosition + direction;
      // Check which adjacent cells are available to place a Dragon
      if (find(availablePositions.begin(), availablePositions.end(), pos) != availablePositions.end()) {
        validDragonPositions.push_back(pos);
      }
    }
    if (validDragonPositions.empty()) continue; // select another hoard position

    Position dragonPosition = validDragonPositions[rng.intRange(static_cast<int>(validDragonPositions.size()))];
    removeAvailablePosition(availablePositions, hoardPosition);
    removeAvailablePosition(availablePositions, dragonPosition);
    place(chamber, hoardPosition, dragonPosition);
    return;
  }
}

void DragonHoardFactory::process(Chamber& chamber, const Position& hoardPosition, const Position& dragonPosition) {
  place(chamber, hoardPosition, dragonPosition); 
}
