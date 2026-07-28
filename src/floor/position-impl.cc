module position;

#ifdef __INTELLISENSE__
#include <compare>
#include <string>

#include "../enums/direction.cc"
#include "position.cc"
#else
import <compare>;
import <string>;
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

Position& Position::operator+=(const Position& other) noexcept {
  this->x += other.x;
  this->y += other.y;
  return *this;
}
Position& Position::operator-=(const Position& other) noexcept {
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

Position::operator std::string() const {
  return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}
