export module dragonhoardfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <set>

#include "../floor/position.cc"
#include "../floor/room.cc"
#include "../utils/rng.cc"
#include "enemy-factory.cc"
#include "factory-base.cc"
#include "floor-region.cc"
#else
import <memory>;
import <set>;
import enemyfactory;
import factorybase;
import floorregion;
import position;
import rng;
import room;
#endif  // __INTELLISENSE__

// A DragonHoardFactory creates and links a Dragon and its DragonHoard.
export class DragonHoardFactory final : public FactoryBase {
  const EnemyFactory& enemyFactory;

 protected:
  // Replace Dragon Hoard placeholders in the Room and place their Dragons.
  virtual void processRoom(FloorRegion& region) override;

 public:
  DragonHoardFactory(
      RNG& rng, const EnemyFactory& enemyFactory,
      std::shared_ptr<FloorPopulationStrategy> strategy = makeDefaultFloorPopulationStrategy());

  // Place a predetermined positioned Dragon and its DragonHoard within the Room.
  void process(Room& room, const Position& hoardPosition, const Position& dragonPosition) const;
};
