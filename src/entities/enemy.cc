export module enemy;

#pragma once

#ifdef __INTELLISENSE__
#include "character.cc"
#include "../enums/action.cc"
#else
import character;
import action;
#endif  // __INTELLISENSE__

// An Enemy represents a Character that is controlled by the game and can move and attack the player.
class Enemy : public Character {
  using Character::Character;
  virtual Action getNextMove() override;
};
