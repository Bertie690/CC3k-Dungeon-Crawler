export module stats;

#pragma once
#ifdef __INTELLISENSE__
#include <limits>
#else
import <limits>;
#endif  // __INTELLISENSE__

// A Stats struct represents the stats of a Character, used to determine the outcome of combat and other interactions.
//
// The addition and subtraction operators are guaranteed to prevent overflow and underflow, respectively.
export struct Stats {
  // Maximum HP
  unsigned int maxHp;
  // Attack
  unsigned int atk;
  // Defense
  unsigned int def;

  // Increment this Stats by another Stats, preventing overflow.
  Stats& operator+=(const Stats& other) {
    static constexpr unsigned int maxUint = std::numeric_limits<unsigned int>::max();

    this->maxHp = this->maxHp > (maxUint - other.maxHp) ? maxUint : this->maxHp + other.maxHp;
    this->atk = this->atk > (maxUint - other.atk) ? maxUint : this->atk + other.atk;
    this->def = this->def > (maxUint - other.def) ? maxUint : this->def + other.def;
    return *this;
  }
  // Decrement this Stats by another Stats, preventing underflow.
  Stats& operator-=(const Stats& other) {
    static constexpr unsigned int minUint = std::numeric_limits<unsigned int>::min();
    this->maxHp = this->maxHp < other.maxHp ? minUint : this->maxHp - other.maxHp;
    this->atk = this->atk < other.atk ? minUint : this->atk - other.atk;
    this->def = this->def < other.def ? minUint : this->def - other.def;
    return *this;
  }
  // Add 2 stats together, preventing overflow.
  Stats operator+(const Stats& other) const {
    Stats result = *this;
    result += other;
    return result;
  }
  // Subtract 2 stats, preventing underflow.
  Stats operator-(const Stats& other) const {
    Stats result = *this;
    result -= other;
    return result;
  }
};
