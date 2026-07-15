export module cell;

#ifdef __INTELLISENSE__
#include "../position.cc"
#else
import position;
#endif  // __INTELLISENSE__

// A Cell represents a single cell on the Floor grid.
// It owns all Entities it contains (sharing ownership with Game for players).
// TODO @ShauryaSuri: Implement
class Cell {
  Position pos;
};