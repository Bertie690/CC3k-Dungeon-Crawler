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

UIAction StdinInputHandler::readRaceCommand() {
  while (true) {
    cout << "Select your race (s=Shade, d=Drow, v=Vampire, g=Goblin, t=Troll) or q to quit: ";
    string token;
    if (!(cin >> token)) {
      exit(0);
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
  cout << "Enter command (no,so,ea,we,ne,nw,se,sw,a <direction>,u <direction>,f,r,q): ";

  string token;
  if (!(cin >> token)) {
    exit(0);
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

  if (token == "a" || token == "u") {
    string dirToken;
    if (!(cin >> dirToken)) {
      exit(0);
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
    exit(0);
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
