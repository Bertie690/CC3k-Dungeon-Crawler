export module staircase;

#pragma once

#ifdef __INTELLISENSE__
#include "../entities/entity.cc"
#else
import entity;
#endif  // __INTELLISENSE__

// A Staircase represents an Entity that allows players to move between floors.
// TODO: Implement floor transitions
export class Staircase final : public ConcreteEntity {
 public:
  using ConcreteEntity::ConcreteEntity;
};