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
  // The position the entity has moved to.
  const Position& to = entity.position;
};

// Request that Game advances to the next Floor.
export struct FloorTransitionEvent {};
