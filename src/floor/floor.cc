export module floor;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../enums/tile-type.cc"
#include "../utils/rng.cc"
#include "cell.cc"
#include "position.cc"
#include "room.cc"
#else
import <memory>;
import <vector>;
import tiletype;
import rng;
import cell;
import position;
import room;
#endif  // __INTELLISENSE__

using namespace std;

export class Floor {
  vector<vector<unique_ptr<Cell>>> cells;
  vector<unique_ptr<Room>> rooms;

  // Check out of bounds helper
  bool isValidPosition(const Position& position) const;

 public:
  static const int HEIGHT = 25;
  static const int WIDTH = 79;

  // A reference to the global game RNG, for use by Cells and Characters on this Floor.
  RNG& rng;

  Floor(RNG& rng);
  Cell& addCell(Position position, TileType tileType);
  Room& addRoom(unique_ptr<Room> room);
  bool hasCell(const Position& position) const;
  // Get a cell by its absolute position.
  const Cell& getCell(const Position& position) const;
  Cell& getCell(const Position& position);
  Room& getRoomAt(const Position& position);
  const Room& getRoomAt(const Position& position) const;
  void runTurn();
};
