export module entity;

#pragma once

#ifdef __INTELLISENSE__
#include "../floor/position.cc"
#else
import position;
#endif  // __INTELLISENSE__

// An Entity represents a game object with a set position on the Floor.
export class Entity {
 public:
  Position position;
  // Return whether this Entity can collide with other Entities.
  virtual bool hasCollision() const;
  virtual ~Entity() = 0;

  Entity(const Position& position) noexcept;
};
