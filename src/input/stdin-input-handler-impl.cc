module stdininputhandler;

#ifdef __INTELLISENSE__
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>

#include "../enums/action.cc"
#include "../enums/direction.cc"
#include "../enums/race-type.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../game/feature-flags.cc"
#include "stdin-input-handler.cc"
#else
import <optional>;
import <iostream>;
import <stdexcept>;
import <string>;
import inputhandler;
import gameevents;
import observer;
import action;
import direction;
import featureflags;
import racetype;
#endif  // __INTELLISENSE__

using namespace std;

std::optional<RaceType> StdinInputHandler::parseRaceSelection(const char raceChar) noexcept {
  switch (raceChar) {
    case 's':
      return RaceType::Shade;
    case 'd':
      return RaceType::Drow;
    case 'v':
      return RaceType::Vampire;
    case 'g':
      return RaceType::Goblin;
    case 't':
      return RaceType::Troll;
    default:
      return std::nullopt;  // TODO: Handle invalid race selection
  }
}

std::optional<Direction> StdinInputHandler::parseDirection(const string& dirStr) noexcept {
  // i hate this so much
  if (dirStr == "no") {
    return Direction::North;
  } else if (dirStr == "so") {
    return Direction::South;
  } else if (dirStr == "ea") {
    return Direction::East;
  } else if (dirStr == "we") {
    return Direction::West;
  } else if (dirStr == "ne") {
    return Direction::NorthEast;
  } else if (dirStr == "nw") {
    return Direction::NorthWest;
  } else if (dirStr == "se") {
    return Direction::SouthEast;
  } else if (dirStr == "sw") {
    return Direction::SouthWest;
  }
  return std::nullopt;  // Default to empty optional
}

std::optional<Direction> StdinInputHandler::parseNumpadDirection(const string& key) noexcept {
  if (key == "7" || key == "\x1b[H" || key == "\x1bOH" || key == "\x1bOw") {
    return Direction::NorthWest;
  }
  if (key == "8" || key == "\x1b[A" || key == "\x1bOx") return Direction::North;
  if (key == "9" || key == "\x1b[5~" || key == "\x1bOy") return Direction::NorthEast;
  if (key == "4" || key == "\x1b[D" || key == "\x1bOt") return Direction::West;
  if (key == "6" || key == "\x1b[C" || key == "\x1bOv") return Direction::East;
  if (key == "1" || key == "\x1b[F" || key == "\x1bOF" || key == "\x1bOq") {
    return Direction::SouthWest;
  }
  if (key == "2" || key == "\x1b[B" || key == "\x1bOr") return Direction::South;
  if (key == "3" || key == "\x1b[6~" || key == "\x1bOs") return Direction::SouthEast;
  return std::nullopt;
}

UIAction StdinInputHandler::readRaceCommand() {
  while (true) {
    cout << "Select your race (s=Shade, d=Drow, v=Vampire, g=Goblin, t=Troll) or q to quit: ";
    string token;
    if (!(cin >> token)) {
      return Quit{};
    }
    if (token == "q") {
      return Quit{};
    }

    if (token.length() == 1) {
      auto raceOpt = parseRaceSelection(token[0]);
      if (raceOpt) {
        awaitingRace = false;
        return RaceSelect{*raceOpt};
      }
      cerr << "Invalid race selection command. Please enter a valid race character." << endl;
    }
  }
}

UIAction StdinInputHandler::readGameCommand() {
  cout << "Enter command (no,so,ea,we,ne,nw,se,sw,a <direction>,u <direction>,f,r,q";
  if (FeatureFlags::enableNumpadInput) {
    cout << ", numpad 1-4,6-9";
  }
  cout << "): ";

  string token;
  if (!(cin >> token)) {
    return Quit{};
  }

  if (token == "q") return Quit{};

  if (token == "f") return FreezeEnemies{};

  if (token == "r") {
    awaitingRace = true;
    return Restart{};
  }

  if (const auto& dir = parseDirection(token)) {
    return Move{*dir};
  }

  if (FeatureFlags::enableNumpadInput) {
    if (const auto& dir = parseNumpadDirection(token)) {
      return Move{*dir};
    }
  }

  if (token == "a" || token == "u") {
    string dirToken;
    if (!(cin >> dirToken)) {
      return Quit{};
    }
    if (const auto& d = parseDirection(dirToken)) {
      if (token == "a") return Attack{*d};
      return UsePotion{*d};
    }
  }
  throw invalid_argument{"Invalid command"};
}

UIAction StdinInputHandler::readGameOverCommand() {
  cout << "> ";
  string token;
  if (!(cin >> token)) {
    return Quit{};
  }
  if (token == "q") {
    return Quit{};
  }
  if (token == "r") {
    awaitingGameOver = false;
    awaitingRace = true;
    return Restart{};
  }
  throw invalid_argument{"Game is over; enter r to restart or q to quit the game."};
}

UIAction StdinInputHandler::readCommand() {
  if (awaitingGameOver) return readGameOverCommand();
  return awaitingRace ? readRaceCommand() : readGameCommand();
}
