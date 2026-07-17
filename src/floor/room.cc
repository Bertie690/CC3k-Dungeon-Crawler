export module room;

#pragma once

#ifdef __INTELLISENSE__
#include <map>
#include <memory>
#include <vector>

#include "../entities/entity.cc"
#include "../enums/direction.cc"
#include "../enums/room-type.cc"
#include "cell.cc"
#include "position.cc"
#else
import <memory>;
import <map>;
import <vector>;
import entity;
import direction;
import roomtype;
import cell;
import position;
#endif  // __INTELLISENSE__

// A Room groups a number of Cells in the same area.
export class Room {
  // The underlying backing storage for the cells, sorted in row-major order.
  std::map<Position, std::unique_ptr<Cell>> cells;

 public:
  Room(std::vector<std::unique_ptr<Cell>> cells);
  std::vector<Cell*> getCells() const;

  // Return the type of this Room.
  virtual RoomType type() const noexcept = 0;

  // Return whether the given position is within this Room.
  bool isInBounds(const Position& position) const;

  // Return whether the given position is occupied (by an Entity and/or unwalkable tile).
  bool isOccupied(const Position& position) const;

  // Obtain a reference to the Cell at the given position.
  Cell& operator[](const Position& position);
  // Obtain a reference to the Cell at the given position.
  const Cell& operator[](const Position& position) const;

  // Return all directions that contain adjacent Cells in this Room.
  // If walkableOnly is true, only return directions that contain unoccupied and traversable Cells.
  const std::vector<Direction>& getAdjacentCells(const Position& position,
                                                 const bool walkableOnly = false) const;

  virtual ~Room() = default;
};
