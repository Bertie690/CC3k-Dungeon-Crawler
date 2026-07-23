export module character:movestrategytype;

#pragma once
#ifdef __INTELLISENSE__
#include <variant>
#else
import <variant>;
#endif  // __INTELLISENSE__

// Enum class representing the different strategies a Character can use to determine its next move.
// Used to allow sharing different strategies across multiple instances.
export enum class CharacterMoveStrategyType {
  PlayerInput,
  Random,
  Dragon,
};
