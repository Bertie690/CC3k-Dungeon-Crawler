export module goldfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <set>
#include <tuple>

#include "../entities/gold-pile.cc"
#include "../enums/gold-size.cc"
#include "../floor/position.cc"
#include "../floor/room.cc"
#include "floor-region.cc"
#include "standard-factory.cc"
#else
import <memory>;
import <tuple>;
import <stdexcept>;
import <set>;
import floorregion;
import goldpile;
import goldsize;
import position;
import room;
import standardfactory;
#endif  // __INTELLISENSE__

// Factory resposible for generating gold.
export class GoldFactory final : public StandardFactory<GoldPile, GoldSize> {
  // Generate a random GoldSize.
  virtual std::tuple<GoldSize> generateArgs() override;

 protected:
 public:
  using StandardFactory<GoldPile, GoldSize>::StandardFactory;
  virtual std::shared_ptr<GoldPile> create(const Position& position, GoldSize size) const override;
};
