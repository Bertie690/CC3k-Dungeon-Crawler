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
  const Direction dir;
};
// Attack the Character at the given position.
export struct Attack {
  // The absolute position that was attacked
  const Position position;
};
// Attack in the given direction.
export struct AttackDirection {
  // The direction of the attack.
  const Direction dir;
};
// Select a race to play as.
export struct RaceSelect {
  // The race to play as.
  const RaceType race;
};
// Use a potion in the given direction.
export struct UsePotion {
  // The direction of the potion to use.
  const Direction dir;
};
// Freeze all enemies.
export struct FreezeEnemies {};
// Quit the game.
export struct Quit {};

// An action the player can take.
export using PlayerAction =
    std::variant<Pass, Move, AttackDirection, UsePotion, RaceSelect, FreezeEnemies, Quit>;
// An action any character can take.
export using Action = std::variant<Pass, Move, Attack, UsePotion>;
