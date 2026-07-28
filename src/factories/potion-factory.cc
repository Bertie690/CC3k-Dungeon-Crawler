export module potionfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../entities/potion.cc"
#include "../enums/potion-type.cc"
#include "../floor/position.cc"
#include "standard-factory.cc"
#else
import <tuple>;
import <memory>;
import potion;
import potiontype;
import position;
import standardfactory;
#endif

// Factory responsible for generating potions.
export class PotionFactory final : public StandardFactory<Potion, PotionType> {
  virtual std::tuple<PotionType> generateArgs() override;

 public:
  using StandardFactory<Potion, PotionType>::StandardFactory;
  virtual std::shared_ptr<Potion> create(const Position& position, PotionType type) const override;
};
