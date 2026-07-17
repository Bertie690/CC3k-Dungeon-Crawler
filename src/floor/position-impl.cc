module position;

#ifdef __INTELLISENSE__
#include <compare>

#include "../enums/direction.cc"
#include "position.cc"
#else
import <compare>;
import direction;
#endif  // __INTELLISENSE__

std::strong_ordering Position::operator<=>(const Position& other) const {
  if (y != other.y) return y <=> other.y;
  return x <=> other.x;
}

Position Position::operator+(const Position& other) const {
  return Position{this->x + other.x, this->y + other.y};
}

Position Position::operator-(const Position& other) const {
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
Position Position::operator+(const Direction dir) const { return *this + fromDirection(dir); }
Position Position::operator-(const Direction dir) const { return *this - fromDirection(dir); }
Position& Position::operator+=(const Direction dir) { return *this += fromDirection(dir); }
Position& Position::operator-=(const Direction dir) { return *this -= fromDirection(dir); }
