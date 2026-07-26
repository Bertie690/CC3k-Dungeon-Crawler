module dragonhoardfactory;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../entities/dragon-hoard.cc"
#include "../entities/enemy.cc"
#include "../entities/entity.cc"
#include "../entities/gold-pile.cc"
#include "../enums/direction.cc"
#include "../enums/gold-size.cc"
#include "../floor/cell.cc"
#include "dragon-hoard-factory.cc"
#else
import <algorithm>;
import <memory>;
import <stdexcept>;
import <vector>;
import cell;
import direction;
import dragonhoard;
import enemy;
import entity;
import goldpile;
import goldsize;
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
  vector<Position> hoardPositions;
  // Find DragonHoard goldSize placeholders in the room
  for (Cell* cell : room.getCells()) {
    for (const shared_ptr<Entity>& entity : cell->getEntities()) {
      const shared_ptr<GoldPile> goldPile = dynamic_pointer_cast<GoldPile>(entity);
      if (goldPile && !dynamic_pointer_cast<DragonHoard>(entity) &&
          goldPile->size == GoldSize::DragonHoard) {
        hoardPositions.push_back(cell->position);
      }
    }
  }

  // Add a Dragon beside each hoard
  for (const Position& hoardPosition : hoardPositions) {
    vector<Position> validDragonPositions;
    // Find available spots beside the hoard
    for (const Direction direction : room.getAdjacentCells(hoardPosition, true)) {
      Position pos = hoardPosition + direction;
      if (find(availablePositions.begin(), availablePositions.end(), pos) !=
          availablePositions.end()) {
        validDragonPositions.push_back(pos);
      }
    }
    if (validDragonPositions.empty()) {
      throw out_of_range{"No valid position next to Dragon Hoard"};
    }

    Position dragonPosition = rng.pick(validDragonPositions);
    removeAvailablePosition(availablePositions, dragonPosition);
    // Replace the dragonhoard size gold piles with DragonHoards
    Cell& hoardCell = room[hoardPosition];
    shared_ptr<GoldPile> placeholder;
    for (const shared_ptr<Entity>& entity : hoardCell.getEntities()) {
      const shared_ptr<GoldPile> goldPile = dynamic_pointer_cast<GoldPile>(entity);
      if (goldPile && !dynamic_pointer_cast<DragonHoard>(entity) &&
          goldPile->size == GoldSize::DragonHoard) {
        placeholder = goldPile;
        break;
      }
    }
    hoardCell.remove(*placeholder);

    place(room, hoardPosition, dragonPosition);
  }
}

void DragonHoardFactory::process(Room& room, const Position& hoardPosition,
                                 const Position& dragonPosition) {
  place(room, hoardPosition, dragonPosition);
}
