export module floorevents;

#pragma once

#ifdef __INTELLISENSE__
#include "../entities/entity.cc"
#include "../floor/position.cc"
#else
import entity;
import position;
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
      : attacker(attacker), defender(defender), result(result), damage(damage), defeated(defeated) {}
};

// Request that Game advances to the next Floor.
export struct FloorTransitionEvent {};

export struct EntityDeathEvent {
  // The position the entity occupied when it died.
  const Position position;

  EntityDeathEvent(const Position& position) : position(position) {}
};
