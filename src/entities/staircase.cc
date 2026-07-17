export module staircase;

#pragma once

#ifdef __INTELLISENSE__
#include "../entities/entity.cc"
#else
import entity;
#endif  // __INTELLISENSE__

// A Staircase represents an Entity that allows players to move between floors.
export class Staircase final : public Entity {
 public:
  using Entity::Entity;
};