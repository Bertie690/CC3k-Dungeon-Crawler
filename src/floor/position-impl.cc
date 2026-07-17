module position;

#ifdef __INTELLISENSE__
#include <compare>

#include "../enums/direction.cc"
#include "position.cc"
#else
import <compare>;
import direction;
#endif  // __INTELLISENSE__

std::strong_ordering Position::operator<=>(const Position& other) const noexcept {
  if (y != other.y) return y <=> other.y;
  return x <=> other.x;
}

Position Position::operator+(const Position& other) const noexcept {
  return Position{this->x + other.x, this->y + other.y};
}

Position Position::operator-(const Position& other) const noexcept {
  return Position{this->x - other.x, this->y - other.y};
}

Position& Position::operator+=(const Position& other) {
  this->x += other.x;
  this->y += other.y;
  return *this;
}
Position& Position::operator-=(const Position& other) {
  this->x -= other.x;
  this->y -= other.y;
  return *this;
}
Position Position::operator+(const Direction dir) const noexcept {
  return *this + fromDirection(dir);
}
Position Position::operator-(const Direction dir) const noexcept {
  return *this - fromDirection(dir);
}
Position& Position::operator+=(const Direction dir) noexcept { return *this += fromDirection(dir); }
Position& Position::operator-=(const Direction dir) noexcept { return *this -= fromDirection(dir); }

Position Position::fromDirection(const Direction dir) noexcept {
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
