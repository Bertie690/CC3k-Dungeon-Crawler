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

export class Player : public Character, public Observer<PlayerActionEvent> {
  Action nextAction{Pass{}};

  // Return the input handler's determined next action.
  Action getNextMove(Floor& floor);
  // Receives and stores the Players action for the upcoming turn.
  virtual void onNotify(const PlayerActionEvent& event) override;

 public:
  using Character::Character;
  int gold = 0;
};
