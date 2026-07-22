export module character:movement;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../enums/action.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/floor.cc"
#include "character-character.cc"
#include "character-move-strategy-type.cc"
#else
import action;
import floor;
import observer;
import gameevents;
import <memory>;
import :character;
import :movestrategytype;
#endif  // __INTELLISENSE__

export class BaseCharacter;  // forward declare for friendship; no need to import

// A CharacterMoveStrategy defines how a Character will choose its next move.
export class CharacterMoveStrategy {
  friend class BaseCharacter;

  // Return the next Action this Character should take, given the current state of the Floor.
  virtual Action getNextMove(Floor& floor, Character& character) = 0;

 public:
  virtual ~CharacterMoveStrategy() = default;
};

export class PlayerInputMoveStrategy final : public CharacterMoveStrategy,
                                             public Observer<PlayerActionEvent> {
  // The input handler's determined next action.
  Action nextAction{Pass{}};

  virtual void onNotify(const PlayerActionEvent& event) override;
  virtual Action getNextMove(Floor& floor, Character& character) override;
};

class RandomMoveStrategy final : public CharacterMoveStrategy {
 private:
  virtual Action getNextMove(Floor& floor, Character& character) override;
};

class StaticMoveStrategy final : public CharacterMoveStrategy {
 private:
  virtual Action getNextMove(Floor& floor, Character& character) override;
};
