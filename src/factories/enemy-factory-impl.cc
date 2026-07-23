module enemyfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>

#include "../entities/merchant.cc"
#include "../entities/character.cc"
#include "enemy-factory.cc"
#else
import <memory>;
import <stdexcept>;
import merchant;
import character;
#endif  // __INTELLISENSE__

using namespace std;

RaceType EnemyFactory::randomRace() {
  int randomInt = rng.intRange(18);
  if (randomInt < 4) return RaceType::Human;
  if (randomInt < 7) return RaceType::Dwarf;
  if (randomInt < 12) return RaceType::Halfling;
  if (randomInt < 14) return RaceType::Elf;
  if (randomInt < 16) return RaceType::Orc;
  return RaceType::Merchant;
}

Stats EnemyFactory::baseStatsFor(RaceType race) const {
  switch (race) {
    case RaceType::Human:
      return Stats{140, 20, 20};
    case RaceType::Dwarf:
      return Stats{100, 20, 30};
    case RaceType::Elf:
      return Stats{140, 30, 10};
    case RaceType::Orc:
      return Stats{180, 30, 25};
    case RaceType::Merchant:
      return Stats{30, 70, 5};
    case RaceType::Dragon:
      return Stats{150, 20, 20};
    case RaceType::Halfling:
      return Stats{100, 15, 20};
    default:
      throw invalid_argument{"Unknown enemy race"};
  }
}

shared_ptr<Enemy> EnemyFactory::create(const Position& position) { return create(position, randomRace()); }

shared_ptr<Enemy> EnemyFactory::create(const Position& position, RaceType race) {
  Stats stats = baseStatsFor(race);
  CharacterMoveStrategyType strategy = CharacterMoveStrategyType::Random;

  if (race == RaceType::Dragon) {
    strategy = CharacterMoveStrategyType::Static;
  }
  if (race == RaceType::Merchant) {
    return make_shared<Merchant>(position, stats, race, strategy);
  }
  // TODO: Apply the race decorators
  return make_shared<Enemy>(position, stats, race, strategy);
}

shared_ptr<Enemy> EnemyFactory::create(const Position& position, char symbol) {
  switch (symbol) {
    case 'H':
      return create(position, RaceType::Human);
    case 'W':
      return create(position, RaceType::Dwarf);
    case 'E':
      return create(position, RaceType::Elf);
    case 'O':
      return create(position, RaceType::Orc);
    case 'M':
      return create(position, RaceType::Merchant);
    case 'D':
      return create(position, RaceType::Dragon);
    case 'L':
      return create(position, RaceType::Halfling);
    default:
      throw invalid_argument{"Unknown enemy race symbol"};
  }
}
