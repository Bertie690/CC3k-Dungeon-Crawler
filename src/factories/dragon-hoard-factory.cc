export module dragonhoardfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>

#include "../floor/position.cc"
#include "../floor/room.cc"
#include "../utils/rng.cc"
#include "enemy-factory.cc"
#include "factory-base.cc"
#else
import <vector>;
import enemyfactory;
import factorybase;
import position;
import rng;
import room;
#endif  // __INTELLISENSE__

// A DragonHoardFactory creates and links a Dragon and its DragonHoard.
export class DragonHoardFactory : public FactoryBase {
  EnemyFactory& enemyFactory;

  void place(Room& room, const Position& hoardPosition, const Position& dragonPosition);

 public:
  DragonHoardFactory(RNG& rng, EnemyFactory& enemyFactory);

  // Replace Dragon Hoard placeholders in the Room and place their Dragons.
  virtual void process(Room& room, std::vector<Position>& availablePositions) override;
  // Place a predetermined positioned Dragon and its DragonHoard within the Room.
  void process(Room& room, const Position& hoardPosition, const Position& dragonPosition);
};
