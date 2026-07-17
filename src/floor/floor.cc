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
  // The height of a single Floor grid.
  static inline const int HEIGHT = 25;
  // The width of a single Floor grid.
  static inline const int WIDTH = 79;

  // A reference to the global game RNG, for use by Cells and Characters on this Floor.
  RNG& rng;

  Floor(RNG& rng);

  // Add a Room to the floor.
  void addRoom(std::unique_ptr<Room> room);

  // Return whether a cell exists at the given position on this Floor.
  bool hasCell(const Position& position) const;

  // Get a cell by its absolute position.
  Cell& getCell(const Position& position);
  // Get a cell by its absolute position.
  const Cell& getCell(const Position& position) const;

  // Get the Room at the given position, throwing an exception if there is none.
  Room& getRoomAt(const Position& position);
  // Get the Room at the given position, throwing an exception if there is none.
  const Room& getRoomAt(const Position& position) const;

  // Attempt to move an Entity from its current position to the given position.
  // Throws an exception if the move is invalid.
  void move(const Entity& entity, const Position& to);

  void runTurn();
};
