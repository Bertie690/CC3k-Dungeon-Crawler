export module stats;

#pragma once
#ifdef __INTELLISENSE__
#include <cmath>
#else
import <cmath>;
#endif  // __INTELLISENSE__


// A Stats struct represents the stats of a Character.
export struct Stats {
  // Maximum HP
  unsigned int maxHp;
  // Attack
  unsigned int atk;
  // Defense
  unsigned int def;

  Stats& operator+=(const Stats& other) {
    this->maxHp += other.maxHp;
    this->atk += other.atk;
    this->def += other.def;
    return *this;
  }
  Stats operator+(const Stats& other) const {
    Stats result = *this;
    result += other;
    return result;
  }
  Stats operator-(const Stats& other) const {
    Stats result = *this;

    result.maxHp = std::max(0u, result.maxHp - other.maxHp);
    result.atk = std::max(0u, result.atk - other.atk);
    result.def = std::max(0u, result.def - other.def);
    return result;
  }
};
