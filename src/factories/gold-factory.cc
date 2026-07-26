export module goldfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../entities/gold-pile.cc"
#include "../enums/gold-size.cc"
#include "../floor/position.cc"
#include "../floor/room.cc"
#include "standard-factory.cc"
#else
import <memory>;
import <stdexcept>;
import <vector>;
import goldpile;
import goldsize;
import position;
import room;
import standardfactory;
#endif  // __INTELLISENSE__

export class GoldFactory : public StandardFactory<GoldPile> {
 public:
  using StandardFactory<GoldPile>::StandardFactory;

  // Randomly select a GoldSize using the required probabilities.
  GoldSize randomGoldSize();
  // Create a random size GoldPile at position per Gold probability distribution.
  virtual std::shared_ptr<GoldPile> create(const Position& position) override;
  // Create a GoldPile of the given size at position.
  std::shared_ptr<GoldPile> create(const Position& position, GoldSize size);
  // Gold placement requires a size selected by the floor generator.
  void process(Room& room, std::vector<Position>& availablePositions) override;
  // Place a GoldPile of the given size in the availablePositions.
  // Necessary since gold type must be chosen before placement in random generation.
  void process(Room& room, std::vector<Position>& availablePositions, GoldSize size);
};
