export module enemy;

#pragma once

#ifdef __INTELLISENSE__
#include "../events/floor-events.cc"
#include "../events/observer.cc"
#include "character.cc"
#else
import character;
import observer;
import floorevents;
#endif  // __INTELLISENSE__

// An Enemy represents a Character that is controlled by the game and can move and attack the player.
export class Enemy : public BaseCharacter, public Subject<EntityDeathEvent> {
  virtual double getAccuracy(const Character& defender) const override;
  virtual void onDeath(Character* killer) override;

 public:
  using BaseCharacter::BaseCharacter;
};
