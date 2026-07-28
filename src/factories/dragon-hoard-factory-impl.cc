module dragonhoardfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <set>
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
import <memory>;
import <set>;
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

DragonHoardFactory::DragonHoardFactory(RNG& rng, const EnemyFactory& enemyFactory,
                                       std::shared_ptr<FloorPopulationStrategy> strategy)
    : FactoryBase{rng, strategy}, enemyFactory{enemyFactory} {}

void DragonHoardFactory::process(Room& room, const Position& hoardPosition,
                                 const Position& dragonPosition) const {
  shared_ptr<Enemy> dragon = enemyFactory.createDragon(dragonPosition, hoardPosition);
  shared_ptr<DragonHoard> hoard = make_shared<DragonHoard>(hoardPosition, dragon);
  room[hoardPosition].add(hoard);
  room[dragonPosition].add(dragon);
}

void DragonHoardFactory::processRoom(FloorRegion& region) {
  Room& room = region.room;
  set<Position>& availablePositions = region.availablePositions;
  vector<Position> hoardPositions;
  // Find DragonHoard goldSize placeholders in the room
  for (const Cell* cell : room.getCells()) {
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
      if (availablePositions.contains(pos)) {
        validDragonPositions.push_back(pos);
      }
    }
    if (validDragonPositions.empty()) {
      throw out_of_range{"No valid position next to Dragon Hoard"};
    }

    Position dragonPosition = rng.pick(validDragonPositions);
    availablePositions.erase(dragonPosition);
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

    process(room, hoardPosition, dragonPosition);
  }
}
