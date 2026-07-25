export module inputhandler;

#pragma once

#ifdef __INTELLISENSE__
#include <iostream>
#include <memory>
#include <stdexcept>
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
import <stdexcept>;
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

export class InputHandler
    : public Subject<PlayerActionEvent, GameQuitEvent, RaceSelectEvent, FreezeEnemiesEvent>,
      public Observer<GameOverEvent> {
 protected:
  bool awaitingGameOver = false;
  // Read a command from the input source and return the corresponding UIAction to take.
  virtual UIAction readCommand() = 0;

  void onInvalidInput(const std::string& message);
  virtual void onNotify(const GameOverEvent& event) override;

 public:
  void processInput();

  virtual ~InputHandler() = default;
};

void InputHandler::onInvalidInput(const std::string& message) {
  // The main input loop catches and prompts for next command
  throw std::invalid_argument{message};
}

void InputHandler::onNotify(const GameOverEvent&) { awaitingGameOver = true; }

void InputHandler::processInput() {
  const UIAction action = readCommand();
  if (const RaceSelect* raceSelect = std::get_if<RaceSelect>(&action)) {
    notify(RaceSelectEvent{raceSelect->race});
  } else if (std::holds_alternative<FreezeEnemies>(action)) {
    notify(FreezeEnemiesEvent{});
  } else if (std::holds_alternative<Restart>(action)) {
    // doesn't emit PlayerActionEvent since we shouldn't run enemy turns
    return;
  } else if (std::holds_alternative<Quit>(action)) {
    notify(GameQuitEvent{true});
  } else {
    notify(PlayerActionEvent{variantCast<Action>(action)});
  }
}