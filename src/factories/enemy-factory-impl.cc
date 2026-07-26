module enemyfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>

#include "../entities/character.cc"
#include "../entities/merchant.cc"
#include "../enums/gold-size.cc"
#include "decorator-chain.cc"
#include "enemy-factory.cc"
#else
import <memory>;
import <stdexcept>;
import decoratorchain;
import merchant;
import goldsize;
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
      return Stats{.maxHp = 140, .atk = 20, .def = 20};
    case RaceType::Dwarf:
      return Stats{.maxHp = 100, .atk = 20, .def = 30};
    case RaceType::Elf:
      return Stats{.maxHp = 140, .atk = 30, .def = 10};
    case RaceType::Orc:
      return Stats{.maxHp = 180, .atk = 30, .def = 25};
    case RaceType::Merchant:
      return Stats{.maxHp = 30, .atk = 70, .def = 5};
    case RaceType::Dragon:
      return Stats{.maxHp = 150, .atk = 20, .def = 20};
    case RaceType::Halfling:
      return Stats{.maxHp = 100, .atk = 15, .def = 20};
    default:
      throw invalid_argument{"Unknown enemy race"};
  }
}

shared_ptr<Character> EnemyFactory::create(const Position& position) {
  return create(position, randomRace());
}

shared_ptr<Character> EnemyFactory::create(const Position& position, RaceType race) {
  if (race == RaceType::Dragon) {
    throw invalid_argument{"Cannot create a Dragon without an associated hoard!"};
  }

  Stats stats = baseStatsFor(race);
  CharacterMoveStrategyType strategy = CharacterMoveStrategyType::Random;

  unique_ptr<Character> enemy;
  if (race == RaceType::Merchant) {
    enemy = make_unique<Merchant>(position, stats, race, strategy);
  } else {
    enemy = make_unique<Enemy>(position, stats, race, strategy);
  }

  DecoratorChain<Character> decoratorChain{move(enemy)};
  switch (race) {
    case RaceType::Dwarf:
      decoratorChain.add<ReverseDrainCharacterDecorator>();
      break;
    case RaceType::Elf:
      decoratorChain.add<AttacksPerTurnCharacterDecorator>(1);
      break;
    case RaceType::Orc:
      decoratorChain.add<RaceTypeDamageMultiplierCharacterDecorator>(RaceType::Goblin, 1.5);
      break;
    case RaceType::Halfling:
      decoratorChain.add<DodgeChanceCharacterDecorator>(0.5);
      break;
    case RaceType::Human:
      decoratorChain.add<GoldOnDeathCharacterDecorator>(
          GoldDrop{.pilesDropped = 2, .pileSize = GoldSize::Normal}, true);
      break;
    case RaceType::Merchant:
      decoratorChain.add<GoldOnDeathCharacterDecorator>(
          GoldDrop{.pilesDropped = 1, .pileSize = GoldSize::MerchantHoard}, true);
    default:
      break;
  }
  return shared_ptr<Character>{decoratorChain.build()};
}

shared_ptr<Enemy> EnemyFactory::createDragon(const Position& dragonPosition,
                                             const Position& hoardPosition) {
  return make_shared<Enemy>(dragonPosition, baseStatsFor(RaceType::Dragon), RaceType::Dragon,
                            make_unique<DragonMoveStrategy>(hoardPosition));
}

shared_ptr<Character> EnemyFactory::create(const Position& position, char symbol) {
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
      throw invalid_argument{"symbol create: use createDragon"};
    case 'L':
      return create(position, RaceType::Halfling);
    default:
      throw invalid_argument{"Unknown enemy race symbol"};
  }
}
