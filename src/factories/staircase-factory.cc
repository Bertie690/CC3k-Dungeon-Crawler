export module staircasefactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../entities/staircase.cc"
#include "../floor/position.cc"
#include "standard-factory.cc"
#else
import <memory>;
import position;
import staircase;
import standardfactory;
#endif  // __INTELLISENSE__

export class StaircaseFactory final : public StandardFactory<Staircase> {
 public:
  using StandardFactory<Staircase>::StandardFactory;
  virtual std::shared_ptr<Staircase> create(const Position& position) const override;
};

std::shared_ptr<Staircase> StaircaseFactory::create(const Position& position) const {
  return std::make_shared<Staircase>(position);
}