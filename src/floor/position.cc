export module position;

#pragma once

#ifdef __INTELLISENSE__
#include <compare>
#include <string>

#include "../enums/direction.cc"
#else
import <string>;
import <compare>;
import direction;
#endif  // __INTELLISENSE__

// A Position represents a vector in 2D integer space.
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

  // vector arithmetic operations

  // Add 2 Positions together, returning the resulting Position.
  Position operator+(const Position& other) const noexcept;
  // Subtract 2 Positions, returning the resulting Position.
  // Notably useful for obtaining relative positions from absolute ones.
  Position operator-(const Position& other) const noexcept;
  // Add 2 Positions together, modifying this Position in place.
  Position& operator+=(const Position& other) noexcept;
  // Subtract 2 Positions, modifying this Position in place.
  // Notably useful for obtaining relative positions from absolute ones.
  Position& operator-=(const Position& other) noexcept;

  // direction related operations

  // Add a Direction to this Position, returning the resulting Position.
  Position operator+(const Direction dir) const noexcept;
  // Subtract a Direction from this Position, returning the resulting Position.
  Position operator-(const Direction dir) const noexcept;
  // Add a Direction to this Position, modifying this Position in place.
  Position& operator+=(const Direction dir) noexcept;
  // Subtract a Direction from this Position, modifying this Position in place.
  Position& operator-=(const Direction dir) noexcept;

  // Convert a Direction to a Position offset.
  static constexpr Position fromDirection(const Direction dir) noexcept;

  operator std::string() const;
};

// needs to be in same file

constexpr Position Position::fromDirection(const Direction dir) noexcept {
  switch (dir) {
    case Direction::North:
      return Position{0, -1};
    case Direction::South:
      return Position{0, 1};
    case Direction::East:
      return Position{1, 0};
    case Direction::West:
      return Position{-1, 0};
    case Direction::NorthEast:
      return Position{1, -1};
    case Direction::SouthEast:
      return Position{1, 1};
    case Direction::SouthWest:
      return Position{-1, 1};
    case Direction::NorthWest:
      return Position{-1, -1};
  }

  // unreachable
  return Position{0, 0};
}
