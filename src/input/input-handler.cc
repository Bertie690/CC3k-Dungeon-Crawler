export module inputhandler;

#pragma once

#ifdef __INTELLISENSE__
#include <iostream>
#include <memory>
#include <string>
#include <variant>

#include "../enums/action.cc"
#include "../enums/direction.cc"
#include "../enums/race-type.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../utils/variant-cast.cc"
#else
import <memory>;
import <string>;
import <variant>;
import <iostream>;
import gameevents;
import observer;
import action;
import direction;
import racetype;
import variantcast;
#endif  // __INTELLISENSE__

export class InputHandler : public Subject<PlayerActionEvent, GameQuitEvent, RaceSelectEvent, FreezeEnemiesEvent> {
 protected:
  // Read a command from the input source and return the corresponding UIAction to take.
  virtual UIAction readCommand() = 0;

  void onInvalidInput(const std::string& message);

 public:
  void processInput();

  virtual ~InputHandler() = default;
};

void InputHandler::onInvalidInput(const std::string& message) {
  std::cerr << message << std::endl;

  // TODO: Use better error handling
  exit(0);
}

void InputHandler::processInput() {
  const UIAction action = readCommand();
  if (const RaceSelect* raceSelect = std::get_if<RaceSelect>(&action)) {
    notify(RaceSelectEvent{raceSelect->race});
  } else if (std::holds_alternative<FreezeEnemies>(action)) {
    notify(FreezeEnemiesEvent{});
  } else if (std::holds_alternative<Quit>(action)) {
    notify(GameQuitEvent{true});
  } else {
    notify(PlayerActionEvent{variantCast<Action>(action)});
  }
}