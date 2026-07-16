export module room;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../entities/entity.cc"
#include "../enums/room-type.cc"
#include "cell.cc"
#include "position.cc"
#else
import <memory>;
import <vector>;
import entity;
import roomtype;
import cell;
import position;
#endif  // __INTELLISENSE__

using namespace std;

// Rooms group Cells belonging to the same Chamber or Passage
// TODO: random available cell selection?
export class Room {
  // Floor owns the Cells
  vector<Cell*> cells;
  Cell& getCell(const Position& position);
  const Cell& getCell(const Position& position) const;

 public:
  Room(const vector<Cell*>& cells);
  const vector<Cell*>& getCells() const;

  virtual RoomType type() const = 0;
  bool isInBounds(const Position& position) const;
  bool isOccupied(const Position& position) const;
  void add(shared_ptr<Entity> entity, const Position& position);
  void remove(Entity& entity, const Position& position);

  virtual ~Room() = default;
};
