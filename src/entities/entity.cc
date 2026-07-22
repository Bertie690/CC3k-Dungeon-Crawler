export module entity;

#pragma once

#ifdef __INTELLISENSE__
#include "../enums/overlap-result.cc"
#include "../floor/position.cc"
#else
import overlapresult;
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

  // Determine what happens when another Entity attempts to enter this Entity's Cell, and trigger any related effects.
  virtual OverlapResult onOverlap(Entity& movingEntity);
};

// A base class for Entities that own their own positions.
export class BaseEntity : public virtual Entity {
  // The backing position.
  Position pos;

 public:
  BaseEntity(const Position& position) noexcept : pos(position) {};
  virtual ~BaseEntity() = 0;

  virtual Position& position() noexcept override;
  virtual const Position& position() const noexcept override;
};

Entity::~Entity() = default;
bool Entity::hasCollision() const { return true; }
OverlapResult Entity::onOverlap(Entity&) {
  return hasCollision() ? OverlapResult::Blocked : OverlapResult::Enter;
}

BaseEntity::~BaseEntity() = default;
Position& BaseEntity::position() noexcept { return this->pos; }
const Position& BaseEntity::position() const noexcept { return this->pos; }