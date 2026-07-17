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

  Entity(const Position& position) noexcept : position(position){};
  virtual ~Entity() = 0;

  // Return whether this Entity can collide with other Entities and block their movement.
  // Only 1 Entity with collision can occupy a Cell at a time.
  virtual bool hasCollision() const {};
};

Entity::~Entity() = default;
bool Entity::hasCollision() const { return true; }