module floor;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>

#include "../entities/gold-pile.cc"
#include "../enums/gold-size.cc"
#include "../enums/overlap-result.cc"
#include "../events/floor-events.cc"
#include "../events/observer.cc"
#include "floor.cc"
#else
import <memory>;
import <stdexcept>;
import <utility>;
import <vector>;
import <variant>;
import floorevents;
import goldsize;
import observer;
import overlapresult;
import goldpile;
#endif  // __INTELLISENSE__

using namespace std;

Floor::Floor(RNG& rng) : rooms(), rng{rng} {}

void Floor::onNotify(const CharacterDeathEvent& event) {
  // re-send the event
  notify(event);

  remove(event.entity);

  // instant gold pickups are handled by the Game class (which subscribes to this)
  const auto& normalPickup = std::get_if<NormalGoldDrop>(&event.goldDrop);
  if (!normalPickup) {
    return;
  }

  for (int i = 0; i < normalPickup->pilesDropped; ++i) {
    getCell(event.position).add(make_shared<GoldPile>(event.position, normalPickup->pileSize));
  }
}

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

vector<Room*> Floor::getRooms() {
  vector<Room*> roomPtrs;

  for (const unique_ptr<Room>& room : rooms) {
    roomPtrs.push_back(room.get());
  }
  return roomPtrs;
}

void Floor::move(Entity& entity, const Position& to) {
  if (!hasCell(to)) {
    throw out_of_range{"Position doesn't exist"};
  }
  Position fromPosition = entity.position();
  Cell& fromCell = getCell(fromPosition);
  Cell& toCell = getCell(to);

  if (!toCell.isWalkable()) throw invalid_argument{"Position is not walkable"};

  shared_ptr<Entity> movingEntity;
  for (const shared_ptr<Entity>& currentEntity : fromCell.getEntities()) {
    if (currentEntity.get() == &entity) {
      movingEntity = currentEntity;
      break;
    }
  }
  if (!movingEntity) throw invalid_argument{"Entity isn't on the Floor"};

  vector<Entity*> consumedEntities;
  for (const shared_ptr<Entity>& destinationEntity : toCell.getEntities()) {
    switch (destinationEntity->onOverlap(entity)) {
      case OverlapResult::Blocked:
        throw invalid_argument{"Position is occupied"};
      case OverlapResult::Enter:
        break;
      case OverlapResult::Consumed:
        consumedEntities.push_back(destinationEntity.get());
        break;
      case OverlapResult::FloorTransition:
        notify(FloorTransitionEvent{});
        return;
    }
  }
  for (Entity* consumedEntity : consumedEntities) {
    toCell.remove(*consumedEntity);
  }

  fromCell.remove(entity);
  entity.position() = to;
  toCell.add(movingEntity);

  notify(EntityMoveEvent{entity, fromPosition});
}

void Floor::remove(const Entity& entity) {
  const Position position = entity.position();
  getCell(position).remove(entity);
  notify(EntityDeathEvent{position});
}

void Floor::reportCharacterAction(const CharacterActionEvent& event) { notify(event); }
