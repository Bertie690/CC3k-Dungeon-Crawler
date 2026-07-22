export module staircase;

#pragma once

#ifdef __INTELLISENSE__
#include "../entities/entity.cc"
#include "../enums/overlap-result.cc"
#else
import entity;
import overlapresult;
#endif  // __INTELLISENSE__

// A Staircase represents an Entity that allows players to move between floors.
// TODO: Implement floor transitions
export class Staircase final : public BaseEntity {
  virtual OverlapResult onOverlap(Entity& movingEntity) override;

 public:
  using BaseEntity::BaseEntity;
};