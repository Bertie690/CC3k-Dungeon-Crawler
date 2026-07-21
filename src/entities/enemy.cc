export module enemy;

#pragma once

#ifdef __INTELLISENSE__
#include "character.cc"
#else
import character;
#endif  // __INTELLISENSE__

// An Enemy represents a Character that is controlled by the game and can move and attack the player.
export class Enemy : public BaseCharacter {
  virtual double getAccuracy(const Character& defender) const override;

 public:  // TODO: remove public access once no longer using enemy demo
  using BaseCharacter::BaseCharacter;
};
