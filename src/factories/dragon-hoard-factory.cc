export module dragonhoardfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>

#include "../floor/chamber.cc"
#include "../floor/position.cc"
#include "../utils/rng.cc"
#include "enemy-factory.cc"
#include "factory-base.cc"
#else
import <vector>;
import chamber;
import enemyfactory;
import factorybase;
import position;
import rng;
#endif  // __INTELLISENSE__

// A DragonHoardFactory creates and links a Dragon and its DragonHoard.
// Unlike the other factories, DragonHoardFactory also handles determining where and placing the entities in the chamber.
export class DragonHoardFactory : public FactoryBase {
  EnemyFactory& enemyFactory;

  void place(Chamber& chamber, const Position& hoardPosition, const Position& dragonPosition);
 public:
  DragonHoardFactory(RNG& rng, EnemyFactory& enemyFactory);

  // Randomly place a Dragon and its DragonHoard together on availablePositions within Chamber
  virtual void process(Chamber& chamber, std::vector<Position>& availablePositions) override;
  // Place a predetermined positioned Dragon and its DragonHoard within Chamber.
  void process(Chamber& chamber, const Position& hoardPosition, const Position& dragonPosition);
};
