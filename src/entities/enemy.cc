export module enemy;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/action.cc"
#include "../floor/floor.cc"
#include "character.cc"
#else
import character;
import action;
import floor;
#endif  // __INTELLISENSE__

// An Enemy represents a Character that is controlled by the game and can move and attack the player.
export class Enemy : public Character {
  using Character::Character;
  virtual Action getNextMove(Floor& floor) override;
};
