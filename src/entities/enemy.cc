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
  virtual double getAccuracy(Character& defender) const override;

 public:  // TODO: remove public access once no longer using enemy demo
  using Character::Character;
};
