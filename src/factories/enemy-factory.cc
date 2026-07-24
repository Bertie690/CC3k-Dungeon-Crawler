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
import enemy;
import stats;
import racetype;
import position;
import standardfactory;
#endif  // __INTELLISENSE__

export class EnemyFactory : public StandardFactory<Character> {

  // Randomly select a enemy RaceType using the required probabilities.
  RaceType randomRace();
  // Return the base stats for the enemy RaceType.
  Stats baseStatsFor(RaceType race) const;
  // Create a chosen RaceType Enemy at position.
  std::shared_ptr<Character> create(const Position& position, RaceType race);

 public:
  using StandardFactory<Character>::StandardFactory;
  // random-gen: Create a randomly chosen RaceType Enemy at position.
  virtual std::shared_ptr<Character> create(const Position& position) override;
  // preset-gen: Create a chosen RaceType Enemy at position using its symbol.
  std::shared_ptr<Character> create(const Position& position, char symbol);
  // Create a Dragon at dragonPosition that guards the Hoard at hoardPosition.
  std::shared_ptr<Enemy> createDragon(const Position& dragonPosition, const Position& hoardPosition);
};
