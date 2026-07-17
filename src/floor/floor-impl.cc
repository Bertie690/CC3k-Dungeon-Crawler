module floor;

#ifdef __INTELLISENSE__
#include <stdexcept>
#include <utility>

#include "floor.cc"
#else
import <stdexcept>;
import <utility>;
#endif  // __INTELLISENSE__

using namespace std;

Floor::Floor(RNG& rng) : rng{rng}, rooms() {}

bool Floor::isInBounds(const Position& position) const {
  return position.x >= 0 && position.y >= 0 && position.x < WIDTH && position.y < HEIGHT;
}

void Floor::addRoom(unique_ptr<Room> room) { rooms.push_back(std::move(room)); }

bool Floor::hasCell(const Position& position) const {
  return this->getRoomAt(position).isInBounds(position);
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

void Floor::move(const Entity& entity, const Position& to) {
  // TODO: Write as part of Implement methods on Floor task
}

void Floor::runTurn() {
  // TODO: Write as part of Implement methods on Floor task
}
