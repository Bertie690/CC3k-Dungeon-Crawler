export module player;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/action.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/floor.cc"
#include "character.cc"
#else
import action;
import gameevents;
import observer;
import floor;
import character;
#endif  // __INTELLISENSE__

export class Player : public BaseCharacter, public Observer<PlayerActionEvent> {
  Action nextAction{Pass{}};

 public:
  using BaseCharacter::BaseCharacter;
  int gold = 0;
};
