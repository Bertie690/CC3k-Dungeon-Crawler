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
import character;
import player;
import stats;
import racetype;
import position;
import standardfactory;
#endif  // __INTELLISENSE__

export class PlayerFactory : public StandardFactory<Character> {
  // Return the base stats for the player RaceType.
  Stats baseStatsFor(RaceType race) const;
 public:
  using StandardFactory<Character>::StandardFactory;
  // Create a default Shade Player at position.
  // note: This isn't used, but is necessary to be a concrete StandardFactory which isn't really necessary for Player..
  virtual std::shared_ptr<Character> create(const Position& position) override;
  // Create a chosen RaceType Player at position.
  std::shared_ptr<Character> create(const Position& position, RaceType race);
};
