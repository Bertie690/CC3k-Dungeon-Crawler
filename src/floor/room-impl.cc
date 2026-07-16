module room;

#ifdef __INTELLISENSE__
#include <stdexcept>

#include "room.cc"
#else
import <stdexcept>;
#endif  // __INTELLISENSE__

using namespace std;

Room::Room(const vector<Cell*>& cells) : cells{cells} {
  if (this->cells.empty()) {
    throw invalid_argument{"A Room requires at least 1 Cell"};
  }
}

Cell& Room::getCell(const Position& position) {
  for (Cell* cell : cells) {
    if (cell->getPosition() == position) return *cell;
  }
  throw out_of_range{"Position not in Room"};
}

const Cell& Room::getCell(const Position& position) const {
  for (const Cell* cell : cells) {
    if (cell->getPosition() == position) return *cell;
  }
  throw out_of_range{"Position not in Room"};
}

bool Room::isInBounds(const Position& position) const {
  for (const Cell* cell : cells) {
    if (cell->getPosition() == position) return true;
  }
  return false;
}

const vector<Cell*>& Room::getCells() const { return cells; }
bool Room::isOccupied(const Position& position) const { return getCell(position).isOccupied(); }
void Room::add(shared_ptr<Entity> entity, const Position& position) {
  getCell(position).add(entity);
}
void Room::remove(Entity& entity, const Position& position) { getCell(position).remove(entity); }
