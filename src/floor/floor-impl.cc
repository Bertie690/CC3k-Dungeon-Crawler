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

Floor::Floor(RNG& rng) : rng{rng} {
  // sets all cells[HEIGHT][WIDTH] = nullptr
  cells.resize(HEIGHT);
  for (vector<unique_ptr<Cell>>& row : cells) {
    row.resize(WIDTH);
  }
}

bool Floor::isValidPosition(const Position& position) const {
  return position.x >= 0 && position.y >= 0 && position.x < WIDTH && position.y < HEIGHT;
}

Cell& Floor::addCell(Position position, TileType tileType) {
  if (!isValidPosition(position)) throw out_of_range{"Invalid cell position"};
  if (hasCell(position)) throw invalid_argument{"Floor position already has cell"};

  unique_ptr<Cell>& cell = cells[position.y][position.x];
  cell = make_unique<Cell>(position, tileType);
  return *cell;
}

Room& Floor::addRoom(unique_ptr<Room> room) {
  // Transfers Room ownership to Floor
  rooms.push_back(std::move(room));
  return *rooms[rooms.size() - 1];
}

bool Floor::hasCell(const Position& position) const {
  if (!isValidPosition(position)) return false;
  return cells[position.y][position.x] != nullptr;
}

Cell& Floor::getCell(const Position& position) {
  if (!hasCell(position)) throw out_of_range{"No cell at this position"};
  return *cells[position.y][position.x];
}
const Cell& Floor::getCell(const Position& position) const {
  if (!hasCell(position)) throw out_of_range{"No cell at this position"};
  return *cells[position.y][position.x];
}

Room& Floor::getRoomAt(const Position& position) {
  for (const unique_ptr<Room>& room : rooms) {
    if (room->isInBounds(position)) return *room;
  }
  throw out_of_range{"No room at this postiion"};
}
const Room& Floor::getRoomAt(const Position& position) const {
  for (const unique_ptr<Room>& room : rooms) {
    if (room->isInBounds(position)) return *room;
  }
  throw out_of_range{"No room at this postiion"};
}

void Floor::runTurn() {
  // TODO: as part of Implement methods on Floor task
}
