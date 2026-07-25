export module action;

#pragma once

#ifdef __INTELLISENSE__
#include <variant>

#include "../floor/position.cc"
#include "direction.cc"
#include "race-type.cc"
#else
import <variant>;
import direction;
import position;
import racetype;
#endif  // __INTELLISENSE__

// Do nothing.
export struct Pass {};
// Move in the given direction.
export struct Move {
  // The direction of movement.
  Direction dir;
};

// Attack the Character at the given position relative to the attacking Character.
export struct Attack {
  // The RELATIVE position that was attacked.
  Position position;

  Attack(const Position& position) : position{position} {}
  Attack(const Direction dir) : position{Position::fromDirection(dir)} {}
  Attack(const Attack& atk) : position{atk.position} {}
};
// Select a race to play as.
export struct RaceSelect {
  // The race to play as.
  RaceType race;
};
// Use a potion in the given direction.
export struct UsePotion {
  // The direction of the potion to use.
  Direction dir;
};
// Freeze all enemies.
export struct FreezeEnemies {};
// Quit the game.
export struct Quit {};

// An action the end user can take with respect to the game.
export using UIAction =
    std::variant<Pass, Move, Attack, UsePotion, RaceSelect, FreezeEnemies, Quit>;
// An action any character can take.
export using Action = std::variant<Pass, Move, Attack, UsePotion>;
