module floor;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../events/floor-events.cc"
#include "../events/observer.cc"
#include "floor.cc"
#else
import <memory>;
import <stdexcept>;
import <utility>;
import <vector>;
import floorevents;
import observer;
#endif  // __INTELLISENSE__

using namespace std;

Floor::Floor(RNG& rng) : rng{rng}, rooms() {}

bool Floor::isInBounds(const Position& position) const {
  return position.x >= 0 && position.y >= 0 && position.x < WIDTH && position.y < HEIGHT;
}

void Floor::addRoom(unique_ptr<Room> room) { rooms.push_back(std::move(room)); }

bool Floor::hasCell(const Position& position) const {
  if (!isInBounds(position)) return false;
  for (const unique_ptr<Room>& room : rooms) {
    if (room->isInBounds(position)) return true;
  }
  return false;
}

Cell& Floor::getCell(const Position& position) { return this->getRoomAt(position)[position]; }
const Cell& Floor::getCell(const Position& position) const {
  return this->getRoomAt(position)[position];
}

Room& Floor::getRoomAt(const Position& position) {
  for (const unique_ptr<Room>& room : rooms) {
    if (room->isInBounds(position)) return *room;
  }
  throw out_of_range{"No room at this position"};
}
const Room& Floor::getRoomAt(const Position& position) const {
  for (const unique_ptr<Room>& room : rooms) {
    if (room->isInBounds(position)) return *room;
  }
  throw out_of_range{"No room at this position"};
}

vector<const Room*> Floor::getRooms() const {
  vector<const Room*> roomPtrs;

  for (const unique_ptr<Room>& room : rooms) {
    roomPtrs.push_back(room.get());
  }
  return roomPtrs;
}

void Floor::move(Entity& entity, const Position& to) {
  if (!hasCell(to)) {
    throw out_of_range{"Position doesn't exist"};
  }
  Position fromPosition = entity.position;
  Cell& fromCell = getCell(fromPosition);
  Cell& toCell = getCell(to);

  if (!toCell.isWalkable()) throw invalid_argument{"Position is not walkable"};
  if (toCell.isOccupied()) throw invalid_argument{"Position is occupied"};

  shared_ptr<Entity> movingEntity;
  for (const shared_ptr<Entity>& currentEntity : fromCell.getEntities()) {
    if (currentEntity.get() == &entity) {
      movingEntity = currentEntity;
      break;
    }
  }
  if (!movingEntity) throw invalid_argument{"Entity isn't on the Floor"};
  fromCell.remove(entity);
  entity.position = to;
  toCell.add(movingEntity);

  notify(EntityMoveEvent{entity, fromPosition});
  // TODO: if Player attempts to move to a staircase Cell, emit FloorTransitionEvent
}
