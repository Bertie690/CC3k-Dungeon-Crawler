module room;

#ifdef __INTELLISENSE__
#include <stdexcept>

#include "room.cc"
#else
import <stdexcept>;
#endif  // __INTELLISENSE__

using namespace std;

Room::Room(const vector<Cell*>& cells) {
  if (cells.empty()) {
    throw invalid_argument{"A Room requires at least 1 Cell"};
  }
  for (Cell* cell : cells) {
    this->cells.emplace(cell->getPosition(), unique_ptr<Cell>{cell});
  }
}

bool Room::isInBounds(const Position& position) const {
  return cells.find(position) != cells.end();
}

const vector<Cell*>& Room::getCells() const {
  vector<Cell*> cellPtrs;
  for (const auto& [_, cell] : cells) {
    cellPtrs.push_back(cell.get());
  }
  return cellPtrs;
}
bool Room::isOccupied(const Position& position) const {
  const Cell& cell = (*this)[position];
  return cell.isOccupied() || !cell.isWalkable();
}

const Cell& Room::operator[](const Position& position) const { return *cells.at(position); }
Cell& Room::operator[](const Position& position) { return *cells.at(position); }