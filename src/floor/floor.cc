export module floor;

#pragma once

#ifdef __INTELLISENSE__
#include "../utils/rng.cc"
#include "cell.cc"
#include "position.cc"
#else
import rng;
import cell;
import position;
#endif  // __INTELLISENSE__

// TODO @ShauryaSuri implement
export class Floor {
 public:
  // A reference to the global game RNG, for use by Cells and Characters on this Floor.
  RNG& rng;
  // Get a cell by its absolute position.
  Cell& getCell(const Position& pos) const;
};
