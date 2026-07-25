export module stdininputhandler;

#ifdef __INTELLISENSE__
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../enums/action.cc"
#include "../enums/direction.cc"
#include "../enums/race-type.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "input-handler.cc"
#else
import <iostream>;
import <memory>;
import <stdexcept>;
import <string>;
import <vector>;
import inputhandler;
import gameevents;
import observer;
import action;
import direction;
import racetype;
#endif  // __INTELLISENSE__

using namespace std;

export class StdinInputHandler : public InputHandler {
 private:
  bool awaitingRace = true;

  bool parseRaceSelection(char raceChar, RaceType& selectedRace) {
    switch (raceChar) {
      case 's':
        selectedRace = RaceType::Shade;
        return true;
      case 'd':
        selectedRace = RaceType::Drow;
        return true;
      case 'v':
        selectedRace = RaceType::Vampire;
        return true;
      case 'g':
        selectedRace = RaceType::Goblin;
        return true;
      case 't':
        selectedRace = RaceType::Troll;
        return true;
      default:
        return false;  // TODO: Handle invalid race selection
    }
  }

  // Parse direction from string
  bool parseDirection(const string& dirStr, Direction& direction) {
    if (dirStr == "no") {
      direction = Direction::North;
      return true;
    }
    if (dirStr == "so") {
      direction = Direction::South;
      return true;
    }
    if (dirStr == "ea") {
      direction = Direction::East;
      return true;
    }
    if (dirStr == "we") {
      direction = Direction::West;
      return true;
    }
    if (dirStr == "ne") {
      direction = Direction::NorthEast;
      return true;
    }
    if (dirStr == "nw") {
      direction = Direction::NorthWest;
      return true;
    }
    if (dirStr == "se") {
      direction = Direction::SouthEast;
      return true;
    }
    if (dirStr == "sw") {
      direction = Direction::SouthWest;
      return true;
    }
    return false;  // Default to North
  }

  PlayerAction readRaceCommand() {
    while (true) {
      cout << "Select your race (s=Shade, d=Drow, v=Vampire, g=Goblin, t=Troll) or q to quit: ";
      string token;
      if (!(cin >> token)) {
        exit(0);
      }
      if (token == "q") {
        exit(0);
      }

      RaceType race;

      if (token.length() == 1 && parseRaceSelection(token[0], race)) {
        awaitingRace = false;
        return RaceSelect{race};
      }
    }
  }

  PlayerAction readGameCommand() {
    cout << "Enter command (no,so,ea,we,ne,nw,se,sw,a <direction>,u <direction>,f,r,q): ";

    string token;
    if (!(cin >> token)) {
      exit(0);
    }

    if (token == "q") exit(0);

    if (token == "f") return FreezeEnemies{};

    if (token == "r") {
      awaitingRace = true;
      return Pass{};
    }

    Direction dir;

    if (parseDirection(token, dir)) {
      return Move{dir};
    }

    if (token == "a" || token == "u") {
      string dirToken;
      if (!(cin >> dirToken)) {
        exit(0);
      }
      if (parseDirection(dirToken, dir)) {
        if (token == "a") return AttackDirection{dir};
        return UsePotion{dir};
      }
    }
    throw invalid_argument{"Invalid command"};
  }

  PlayerAction readCommand() override {
    return awaitingRace ? readRaceCommand() : readGameCommand();
  }

 public:
  void processInput() override {
    PlayerAction action = readCommand();
    notify(PlayerActionEvent{action});
  }
};
