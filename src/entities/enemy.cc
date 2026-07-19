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
 public:  // TODO: remove public access once no longer using enemy demo
  using Character::Character;

 private:  // TODO: remove private keyword once no longer using enemy demo
  virtual Action getNextMove(Floor& floor) override;
};
