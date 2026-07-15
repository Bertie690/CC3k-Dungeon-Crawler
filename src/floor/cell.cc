export module cell;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../entities/entity.cc"
#include "floor.cc"
#include "position.cc"
#else
import position;
import entity;
import floor;
import <vector>;
import <memory>;
#endif  // __INTELLISENSE__

// A Cell represents a single cell on the Floor grid.
// It owns all Entities it contains (sharing ownership with Game for players).
// TODO @ShauryaSuri: Implement methods and such
class Cell {
 public:
  Position pos;
  // Get a reference to the Floor this Cell is on.
  Floor& getFloor() const;

  // Get a reference to the Character located at the current Cell, if one exists.
  const Character* const getCharacter() const;
};
