export module enemyfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../entities/character.cc"
#include "../entities/enemy.cc"
#include "../entities/stats.cc"
#include "../floor/position.cc"
#include "standard-factory.cc"
#include "../enums/race-type.cc"

#else
import <memory>;
import character;
import <tuple>;
import enemy;
import stats;
import racetype;
import position;
import standardfactory;
#endif  // __INTELLISENSE__

export class EnemyFactory final : public StandardFactory<Character, RaceType> {
  // Return the base stats for the enemy RaceType.
  Stats baseStatsFor(RaceType race) const;

  // Generate a random RaceType.
  virtual std::tuple<RaceType> generateArgs() override;

 protected:
 public:
  using StandardFactory<Character, RaceType>::StandardFactory;
  virtual std::shared_ptr<Character> create(const Position& position, RaceType race) const override;
  // Create a Dragon at dragonPosition that guards the Hoard at hoardPosition.
  std::shared_ptr<Enemy> createDragon(const Position& dragonPosition,
                                      const Position& hoardPosition) const;
};
