module cell;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "cell.cc"
#else
import <memory>;
import <stdexcept>;
import <utility>;
import <vector>;
#endif  // __INTELLISENSE__

using namespace std;

Cell::Cell(Position position, TileType tileType) : position{position}, tileType{tileType} {}
const Position& Cell::getPosition() const { return position; }
TileType Cell::getTileType() const { return tileType; }
const vector<shared_ptr<Entity>>& Cell::getEntities() const { return entities; }
bool Cell::isWalkable() const {
  return tileType != TileType::VerticalWall && tileType != TileType::HorizontalWall;
}

bool Cell::isOccupied() const {
  for (const auto& entity : entities) {
    if (entity->hasCollision()) {
      return true;
    }
  }
  return false;
}

void Cell::add(std::shared_ptr<Entity> entity) {
  for (const std::shared_ptr<Entity>& cur : entities) {
    if (cur.get() == entity.get()) {
      throw std::invalid_argument{"Entity is already in Cell"};
    }
  }
  entities.push_back(std::move(entity));
}

void Cell::remove(std::shared_ptr<Entity> entity) {
  for (auto it = entities.begin(); it != entities.end(); ++it) {
    if (it->get() == entity.get()) {
      entities.erase(it);
      return;
    }
  }
  throw std::invalid_argument{"Entity isn't in this Cell"};
}
