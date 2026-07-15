export module position;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/direction.cc"
#else
import direction;
#endif  // __INTELLISENSE__

// A Position represents a vector in Z^2.
export struct Position {
  // The X coordinate. Positive is right, negative is left.
  int x;
  // The Y coordinate. Positive is down, negative is up.
  int y;

  // TODO: Add more overloads if we need em

  bool operator==(const Position& other) const;
  bool operator!=(const Position& other) const;

  Position operator+(const Position& other) const;
  Position operator-(const Position& other) const;
  Position& operator+=(const Position& other);
  Position& operator-=(const Position& other);

  // direction related operations

  Position operator+(const Direction dir) const;
  Position operator-(const Direction dir) const;
  Position& operator+=(const Direction dir);
  Position& operator-=(const Direction dir);

  // Convert a Direction to a Position offset.
  static Position fromDirection(const Direction dir);
};
