export module inputhandler;

#pragma once

#ifdef __INTELLISENSE__
#include <iostream>
#include <memory>
#include <string>

#include "../enums/action.cc"
#include "../enums/direction.cc"
#include "../enums/race-type.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#else
import <memory>;
import <string>;
import <iostream>;
import gameevents;
import observer;
import action;
import direction;
import racetype;
#endif  // __INTELLISENSE__

export class InputHandler : public Observer<PlayerActionEvent, GameQuitEvent, RaceSelectEvent> {
 protected:
  virtual PlayerAction readCommand() = 0;

 public:
  virtual void processInput() = 0;

  virtual void onNotify(const PlayerActionEvent& event) override {}
  virtual void onNotify(const GameQuitEvent& event) override {}
  virtual void onNotify(const RaceSelectEvent& event) override {}

  virtual ~InputHandler() = default;
};
