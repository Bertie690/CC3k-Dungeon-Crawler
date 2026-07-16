export module enemy;

#pragma once

#ifdef __INTELLISENSE__
#include "character.cc"
#include "../enums/action.cc"
#else
import character;
import action;
#endif  // __INTELLISENSE__

class Enemy : public Character {
  virtual Action getNextMove() override;
};
