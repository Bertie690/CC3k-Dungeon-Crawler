export module floorevents;

#pragma once

#ifdef __INTELLISENSE__
#include <variant>

#include "../entities/entity.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../floor/position.cc"
#else
import entity;
import <variant>;
import position;
import goldsize;
import racetype;
#endif  // __INTELLISENSE__

export struct EntityMoveEvent {
  // The entity that has moved.
  const Entity& entity;
  // The position the entity had moved from.
  const Position from;
  // The position the entity has moved to. Pure syntactic sugar for the entity's current position.
  const Position& to = entity.position();

  EntityMoveEvent(const Entity& entity, const Position& from) : entity(entity), from(from) {}
};

export struct CharacterActionEvent {
  enum class Result {
    Hit,
    Miss,
  };

  const Entity& attacker;
  const Entity& defender;
  const Result result;
  const unsigned int damage;
  const bool defeated;

  CharacterActionEvent(const Entity& attacker, const Entity& defender, Result result,
                       unsigned int damage = 0, bool defeated = false)
      : attacker(attacker),
        defender(defender),
        result(result),
        damage(damage),
        defeated(defeated) {}
};

// Request that Game advances to the next Floor.
export struct FloorTransitionEvent {};

export struct EntityDeathEvent {
  // The position the entity occupied when it died.
  const Position position;

  EntityDeathEvent(const Position& position) : position(position) {}
};

// Event struct emitted when a Character dies, in addition to the associated EntityDeathEvent.
//
// Contains relevant information about the Character (and not the actual object itself) to avoid
// circular dependencies with Character itself and abide by the principle of least privilege.
export struct CharacterDeathEvent {
  // The entity that has died.
  // This is always a Character, but is typed as an Entity to avoid circular dependencies with Character itself.
  const Entity& entity;

  // The position of the Character that just died.
  const Position& position;

  // The position of the Character that killed the defender.
  // If the Character died from a non-Character source (e.g. a trap), this will be the same as position.
  const Position killerPosition;

  // The race of the Character that just died.
  const RaceType raceType;

  // Information pertaining to the gold dropped by the Character upon death.
  GoldDrop goldDrop;

  // gold awarded directly to the killer.
  GoldDrop killerGoldDrop;

  CharacterDeathEvent(const Entity& entity, const Position& killerPosition, const RaceType raceType,
                      GoldDrop goldDrop, GoldDrop killerGoldDrop)
      : entity(entity),
        position(entity.position()),
        killerPosition(killerPosition),
        raceType(raceType),
        goldDrop(goldDrop),
        killerGoldDrop(killerGoldDrop) {}
};
