module cell;

#ifdef __INTELLISENSE__
#include <stdexcept>

#include "cell.cc"
#else
import <stdexcept>;
#endif  // __INTELLISENSE__

using namespace std;

Cell::Cell(Position position, TileType tileType) : position{position}, tileType{tileType} {}
const Position& Cell::getPosition() const { return position; }
TileType Cell::getTileType() const { return tileType; }
const vector<shared_ptr<Entity>>& Cell::getEntities() const { return entities; }
bool Cell::isWalkable() const {
  return tileType != TileType::VerticalWall && tileType != TileType::HorizontalWall;
}
bool Cell::isOccupied() const { return !entities.empty(); }

void Cell::add(shared_ptr<Entity> entity) {
  if (!entity) {
    throw invalid_argument{"Can't add invalid entity"};
  }
  for (const shared_ptr<Entity>& cur : entities) {
    if (cur == entity) {
      throw invalid_argument{"Entity is already in Cell"};
    }
  }
  entities.push_back(entity);
}

void Cell::remove(Entity& entity) {
  for (auto it = entities.begin(); it != entities.end(); ++it) {
    if (it->get() == &entity) {
      entities.erase(it);
      return;
    }
  }
  throw invalid_argument{"Entity isn't in this Cell"};
}
