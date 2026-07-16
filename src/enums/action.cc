export module action;

#pragma once

#ifdef __INTELLISENSE__
#include <variant>

#include "direction.cc"
#include "race-type.cc"
#else
import <variant>;
import direction;
import racetype;
#endif  // __INTELLISENSE__

export struct Pass {};
export struct Move {
  Direction dir;
};
export struct Attack {
  Direction dir;
};
export struct RaceSelect {
  RaceType race;
};
export struct UsePotion {
  Direction dir;
};
export struct FreezeEnemies {};
export struct Quit {};

// An action the player can take.
export using PlayerAction =
    std::variant<Pass, Move, Attack, UsePotion, RaceSelect, FreezeEnemies, Quit>;
// An action any character can take.
export using Action = std::variant<Pass, Move, Attack, UsePotion>;