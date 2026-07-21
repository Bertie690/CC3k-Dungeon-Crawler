export module character:movestrategytype;

#pragma once

// Enum class representing the different strategies a Character can use to determine its next move.
// Used to allow sharing different strategies across multiple instances.
export enum class CharacterMoveStrategyType {
  PlayerInput,
  Random,
  Static,
};