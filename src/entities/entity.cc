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
  virtual ~Entity() = 0;

  // Return the current position of this Entity.
  virtual Position& position() noexcept = 0;
  virtual const Position& position() const noexcept = 0;

  // Return whether this Entity can collide with other Entities and block their movement.
  // Only 1 Entity with collision can occupy a Cell at a time.
  virtual bool hasCollision() const;
};

// An entity that owns its own position.
// Still technically virtual
export class ConcreteEntity : public virtual Entity {
  // The backing position.
  Position pos;

 public:
  ConcreteEntity(const Position& position) noexcept : pos(position) {};
  virtual ~ConcreteEntity() = default;

  virtual Position& position() noexcept override;
  virtual const Position& position() const noexcept override;
};

Entity::~Entity() = default;
bool Entity::hasCollision() const { return true; }

Position& ConcreteEntity::position() noexcept { return this->pos; }
const Position& ConcreteEntity::position() const noexcept { return this->pos; }