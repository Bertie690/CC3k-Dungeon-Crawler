module playerfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>

#include "../entities/character.cc"
#include "../entities/player.cc"
#include "../enums/gold-size.cc"
#include "decorator-chain.cc"
#include "player-factory.cc"
#include <limits>
#else
import <limits>;
import <memory>;
import <stdexcept>;
import character;
import decoratorchain;
import goldsize;
import player;
#endif  // __INTELLISENSE__

using namespace std;

Stats PlayerFactory::baseStatsFor(RaceType race) const {
  switch (race) {
    case RaceType::Shade:
      return Stats{.maxHp = 125, .atk = 25, .def = 25};
    case RaceType::Drow:
      return Stats{.maxHp = 150, .atk = 25, .def = 15};
    case RaceType::Vampire:
      return Stats{.maxHp = 50, .atk = 25, .def = 25};
    case RaceType::Troll:
      return Stats{.maxHp = 120, .atk = 25, .def = 15};
    case RaceType::Goblin:
      return Stats{.maxHp = 110, .atk = 15, .def = 20};
    default:
      throw invalid_argument{"Unknown player race"};
  }
}

tuple<RaceType> PlayerFactory::generateArgs() { return make_tuple(RaceType::Shade); }

shared_ptr<Character> PlayerFactory::create(const Position& position, RaceType race) const {
  Stats stats = baseStatsFor(race);
  unique_ptr<Character> player = make_unique<Player>(position, stats, race);
  DecoratorChain<Character> decoratorChain{move(player)};
  switch (race) {
    case RaceType::Shade:
      // TODO: handle multi in end of game scoring flow
      decoratorChain.add<ScoreMultiCharacterDecorator>(1.5);
      break;
    case RaceType::Drow:
      // TODO: add to potion implementation
      decoratorChain.add<PotionEffectCharacterDecorator>(1.5);
      break;
    case RaceType::Vampire:
      decoratorChain
        .add<HpDrainCharacterDecorator>(5)
        .add<StatChangeCharacterDecorator>(Stats{.maxHp = std::numeric_limits<unsigned int>::max()}, true);
      break;
    case RaceType::Troll:
      decoratorChain.add<TurnHpRegenCharacterDecorator>(5);
      break;
    case RaceType::Goblin:
      decoratorChain.add<GoldOnKillCharacterDecorator>(5);
      break;
    default:
      break;
  }
  return shared_ptr<Character>{decoratorChain.build()};
}
