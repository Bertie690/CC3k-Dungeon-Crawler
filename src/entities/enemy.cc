export module enemy;

#pragma once

#ifdef __INTELLISENSE__
#include "character.cc"
#else
import character;
#endif  // __INTELLISENSE__

class Enemy : public Character {
  virtual Action getNextMove() override;
};
