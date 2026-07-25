export module dragonhoard;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../enums/overlap-result.cc"
#include "../events/floor-events.cc"
#include "../events/observer.cc"
#include "../floor/position.cc"
#include "character.cc"
#include "enemy.cc"
#include "entity.cc"
#include "gold-pile.cc"
#else
import <memory>;
import character;
import entity;
import enemy;
import floorevents;
import goldpile;
import observer;
import overlapresult;
import position;
#endif  // __INTELLISENSE__

// A pile of gold guarded by a dragon. The player cannot pick up the gold until the dragon is dead.
export class DragonHoard : public GoldPile, public Observer<EntityDeathEvent> {
  bool guarded = true;
  std::shared_ptr<Enemy> dragon;

  virtual bool canPickup(Character& character) const override;
  virtual OverlapResult onOverlap(Entity& movingEntity) override;
  virtual void onNotify(const EntityDeathEvent& event) override;

 public:
  DragonHoard(Position position, const std::shared_ptr<Enemy>& dragon);
  virtual ~DragonHoard();
};
