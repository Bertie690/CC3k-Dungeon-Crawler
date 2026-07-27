export module stdininputhandler;

#ifdef __INTELLISENSE__
#include <string>
#include <optional>

#include "../enums/action.cc"
#include "../enums/direction.cc"
#include "../enums/race-type.cc"
#include "../events/observer.cc"
#include "input-handler.cc"
#else
import <optional>;
import <string>;
import inputhandler;
import observer;
import action;
import direction;
import racetype;
#endif  // __INTELLISENSE__

export class StdinInputHandler final : public InputHandler {
 private:
  // Whether the handler is currently awaiting a race selection from the player.
  bool awaitingRace = true;

  std::optional<RaceType> parseRaceSelection(const char raceChar) noexcept;

  std::optional<Direction> parseDirection(const std::string& dirStr) noexcept;

  std::optional<Direction> parseNumpadDirection(const std::string& key) noexcept;

  // Read a race command from stdin.
  UIAction readRaceCommand();

  // Read a game command from stdin.
  UIAction readGameCommand();

  // Read a restart or quit command after the game ends.
  UIAction readGameOverCommand();

  virtual UIAction readCommand() override;
};
