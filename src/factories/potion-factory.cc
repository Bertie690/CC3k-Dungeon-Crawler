export module potionfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../entities/potion.cc"
#include "../enums/potion-type.cc"
#include "../floor/position.cc"
#include "standard-factory.cc"
#else
import <memory>;
import potion;
import potiontype;
import position;
import standardfactory;
#endif

export class PotionFactory : public StandardFactory<Potion> {
 public:
  using StandardFactory<Potion>::StandardFactory;
  virtual std::shared_ptr<Potion> create(const Position& position) override;
  std::shared_ptr<Potion> create(const Position& position, PotionType type);
};
