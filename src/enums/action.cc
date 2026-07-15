export module action;

#ifdef __INTELLISENSE__
#include <variant>

#include "../position.cc"
#include "race-type.cc"
#else
import <variant>;
import position;
import racetype;
#endif;  // __INTELLISENSE__

export struct Pass {};
export struct Move {
  Position pos;
};
export struct Attack {
  Position pos;
};
export struct RaceSelect {
  RaceType race;
};
export struct FreezeEnemies {};
export struct Quit {};

// An action the player can take.
// Any members not included inside Action are not valid for the player to take.
export using PlayerAction = std::variant<Pass, Move, Attack, RaceSelect, FreezeEnemies, Quit>;
// An action any character can take as a turn.
export using Action = std::variant<Pass, Move, Attack>;