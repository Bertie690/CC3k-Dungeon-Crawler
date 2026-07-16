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

  // Compare 2 Positions by their coordinates (row-major order).
  std::strong_ordering operator<=>(const Position& other) const = default;
  bool operator==(const Position& other) const = default;
  bool operator!=(const Position& other) const = default;
  bool operator<(const Position& other) const = default;
  bool operator>(const Position& other) const = default;
  bool operator<=(const Position& other) const = default;
  bool operator>=(const Position& other) const = default;

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
