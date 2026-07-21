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

// Request that Game advances to the next Floor.
export struct FloorTransitionEvent {};
