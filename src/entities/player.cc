export module player;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/race-type.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/position.cc"
#include "character.cc"
#include "stats.cc"
#else
import character;
import gameevents;
import observer;
import position;
import racetype;
import stats;
#endif  // __INTELLISENSE__

export class Player : public BaseCharacter {
 public:
  // Explicit constructor to guarantee Player strategy type.

  Player(Position position, Stats baseStats, RaceType raceType);

  // Expose the Player's strategy so the input handler can use it as an observer.
  virtual Observer<PlayerActionEvent>* inputObserver() override;
  // Return Player's gold.
  virtual int getGold() const override;
  // Add amount to Player's gold.
  virtual void addGold(int amount) override;

  int gold = 0;
  // TODO: Add game over handling
};
