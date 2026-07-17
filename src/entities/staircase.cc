export module staircase;

#pragma once

#ifdef __INTELLISENSE__
#include "../entities/entity.cc"
#else
import entity;
#endif  // __INTELLISENSE__

export class Staircase final : public Entity {
 public:
  Staircase() = default;
  ~Staircase() = default;
};