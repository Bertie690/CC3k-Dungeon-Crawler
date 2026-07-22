export module goldfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include "../entities/gold-pile.cc"
#include "../enums/gold-size.cc"
#include "../floor/position.cc"
#include "standard-factory.cc"
#else
import <memory>;
import goldpile;
import goldsize;
import position;
import standardfactory;
#endif  // __INTELLISENSE__

export class GoldFactory : public StandardFactory<GoldPile> {
  // Randomly select a GoldSize using the required probabilities.
  GoldSize randomGoldSize();

 public:
  using StandardFactory<GoldPile>::StandardFactory;

  // Create a random size GoldPile at position per Gold probability distribution.
  virtual std::shared_ptr<GoldPile> create(const Position& position) override;
  // Create a GoldPile of the given size at position.
  std::shared_ptr<GoldPile> create(const Position& position, GoldSize size);
};
