module position;

#ifdef __INTELLISENSE__
#include "../enums/direction.cc"
#include "position.cc"
#endif  // __INTELLISENSE__

bool Position::operator==(const Position& other) const {
  return this->x == other.x && this->y == other.y;
}
bool Position::operator!=(const Position& other) const { return !(*this == other); }

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