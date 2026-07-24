module playerfactory;

#ifdef __INTELLISENSE__
#include <memory>
#include <stdexcept>
#include "../entities/character.cc"
#include "../entities/player.cc"
#include "decorator-chain.cc"
#include "player-factory.cc"
#else
import <memory>;
import <stdexcept>;
import character;
import decoratorchain;
import player;
#endif  // __INTELLISENSE__

using namespace std;

Stats PlayerFactory::baseStatsFor(RaceType race) const {
  switch (race) {
    case RaceType::Shade:
      return Stats{125, 25, 25};
    case RaceType::Drow:
      return Stats{150, 25, 15};
    case RaceType::Vampire:
      return Stats{50, 25, 25};
    case RaceType::Troll:
      return Stats{120, 25, 15};
    case RaceType::Goblin:
      return Stats{110, 15, 20};
    default:
      throw invalid_argument{"Unknown player race"};
  }
}

shared_ptr<Character> PlayerFactory::create(const Position& position) {
  return create(position, RaceType::Shade);
}

shared_ptr<Character> PlayerFactory::create(const Position& position, RaceType race) {
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
      decoratorChain.add<HpDrainCharacterDecorator>(5);
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
