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

Cell::Cell(const Position& position, const TileType tileType)
    : position{position}, tileType{tileType} {}

// Out of line dtor: workaround for a compiler bug from g++-15.2.0
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=120499
Cell::~Cell() = default;

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

void Cell::remove(const Entity& entity) {
  for (auto it = entities.begin(); it != entities.end(); ++it) {
    if (it->get() == &entity) {
      entities.erase(it);
      return;
    }
  }
  throw std::invalid_argument{"Entity isn't in this Cell"};
}
