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

export class Floor {
  std::vector<std::unique_ptr<Room>> rooms;

  // Return whether the given position is valid on this Floor.
  bool isInBounds(const Position& position) const;

 public:
  static inline const int HEIGHT = 25;
  static inline const int WIDTH = 79;

  // A reference to the global game RNG, for use by Cells and Characters on this Floor.
  RNG& rng;

  Floor(RNG& rng);

  // Add a Room to the floor.
  void addRoom(std::unique_ptr<Room> room);

  // Get a cell by its absolute position.
  Cell& getCell(const Position& position);
  // Get a cell by its absolute position.
  const Cell& getCell(const Position& position) const;

  // Get the Room at the given position, throwing an exception if there is none.
  Room& getRoomAt(const Position& position);
  // Get the Room at the given position, throwing an exception if there is none.
  const Room& getRoomAt(const Position& position) const;

  void runTurn();
};
