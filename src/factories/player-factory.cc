export module playerfactory;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include "../entities/character.cc"
#include "../entities/player.cc"
#include "../entities/stats.cc"
#include "../floor/position.cc"
#include "../enums/race-type.cc"
#include "standard-factory.cc"
#else
import <memory>;
import <tuple>;
import character;
import player;
import stats;
import racetype;
import position;
import standardfactory;
#endif  // __INTELLISENSE__

// Factory responsible for generating the player character.
// Unlike other factories, this does not (currently) partake in random generation, as the player character is stored/created inside Game.
//
// It still conforms to the same interface regardless.
export class PlayerFactory final : public StandardFactory<Character, RaceType> {
  // Return the base stats for the player RaceType.
  Stats baseStatsFor(RaceType race) const;

  // Arbitrary dummy implementation to satisfy StandardFactory
  virtual std::tuple<RaceType> generateArgs() override;

 public:
  using StandardFactory<Character, RaceType>::StandardFactory;

  virtual std::shared_ptr<Character> create(const Position& position, RaceType race) const override;
};
