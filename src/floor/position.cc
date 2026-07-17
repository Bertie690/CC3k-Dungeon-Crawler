export module position;

#pragma once

#ifdef __INTELLISENSE__
#include <compare>

#include "../enums/direction.cc"
#else
import <compare>;
import direction;
#endif  // __INTELLISENSE__

// A Position represents a vector in Z^2.
export struct Position {
  // The X coordinate. Positive is right, negative is left.
  int x;
  // The Y coordinate. Positive is down, negative is up.
  int y;

  // Compare 2 Positions by their coordinates (row-major order).
  std::strong_ordering operator<=>(const Position& other) const noexcept;
  bool operator==(const Position& other) const noexcept = default;
  bool operator!=(const Position& other) const noexcept = default;
  bool operator<(const Position& other) const noexcept = default;
  bool operator>(const Position& other) const noexcept = default;
  bool operator<=(const Position& other) const noexcept = default;
  bool operator>=(const Position& other) const noexcept = default;

  Position operator+(const Position& other) const noexcept;
  Position operator-(const Position& other) const noexcept;
  Position& operator+=(const Position& other) noexcept;
  Position& operator-=(const Position& other) noexcept;

  // direction related operations

  Position operator+(const Direction dir) const noexcept;
  Position operator-(const Direction dir) const noexcept;
  Position& operator+=(const Direction dir) noexcept;
  Position& operator-=(const Direction dir) noexcept;

  // Convert a Direction to a Position offset.
  static Position fromDirection(const Direction dir) noexcept;
};
