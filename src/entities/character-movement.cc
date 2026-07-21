export module character:movement;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../enums/action.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/floor.cc"
#include "character.cc"
#else
import action;
import floor;
import observer;
import gameevents;
import <memory>;
#endif  // __INTELLISENSE__

export class Character;

// Enum class representing the different strategies a Character can use to determine its next move.
// Used to allow sharing different strategies across multiple instances.
export enum class CharacterMoveStrategyType {
  PlayerInput,
  Random,
  Static,
};

// A CharacterMoveStrategy defines how a Character will choose its next move.
export class CharacterMoveStrategy {
  friend class Character;

  // Return the next Action this Character should take, given the current state of the Floor.
  virtual Action getNextMove(Floor& floor, Character& character) = 0;

 public:
  virtual ~CharacterMoveStrategy() = default;
};

export class PlayerInputMoveStrategy final : public CharacterMoveStrategy,
                                             public Observer<PlayerActionEvent> {
  // TODO: Implement player input move strategy
  virtual void onNotify(const PlayerActionEvent& event) override;
  virtual Action getNextMove(Floor& floor, Character& character);
};

export class RandomMoveStrategy final : public CharacterMoveStrategy {
 private:
  virtual Action getNextMove(Floor& floor, Character& character);
};

export class StaticMoveStrategy final : public CharacterMoveStrategy {
 private:
  virtual Action getNextMove(Floor& floor, Character& character);
};
