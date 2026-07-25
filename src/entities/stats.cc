export module stats;

#pragma once
#ifdef __INTELLISENSE__
#include <limits>
#else
import <limits>;
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
    static inline constexpr unsigned int maxUint = std::numeric_limits<unsigned int>::max();

    this->maxHp = this->maxHp + other.maxHp < this->maxHp ? maxUint : this->maxHp + other.maxHp;
    this->atk = this->atk + other.atk < this->atk ? maxUint : this->atk + other.atk;
    this->def = this->def + other.def < this->def ? maxUint : this->def + other.def;
    return *this;
  }
  Stats operator+(const Stats& other) const {
    Stats result = *this;
    result += other;
    return result;
  }
  Stats operator-(const Stats& other) const {
    static inline constexpr unsigned int minUint = std::numeric_limits<unsigned int>::min();
    Stats result = *this;

    result.maxHp = result.maxHp < other.maxHp ? minUint : result.maxHp - other.maxHp;
    result.atk = result.atk < other.atk ? minUint : result.atk - other.atk;
    result.def = result.def < other.def ? minUint : result.def - other.def;
    return result;
  }
};
