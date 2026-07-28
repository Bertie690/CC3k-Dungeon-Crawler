module potion;

#ifdef __INTELLISENSE__
#include "../enums/race-type.cc"
#include "potion.cc"
#else
import <cmath>;
import racetype;
#endif

void Potion::use(Character& character) const {
  const unsigned int amount =
      static_cast<unsigned int>(std::ceil(10.0 * character.getPotionEffectMultiplier()));
  const int statAmount = static_cast<int>(std::ceil(5.0 * character.getPotionEffectMultiplier()));
  switch (potionType) {
    case PotionType::RestoreHealth:
      character.heal(amount);
      break;
    case PotionType::PoisonHealth:
      character.damage(amount, false);
      break;
    case PotionType::BoostAttack:
      character.addTemporaryStats(statAmount, 0);
      break;
    case PotionType::BoostDefense:
      character.addTemporaryStats(0, statAmount);
      break;
    case PotionType::WoundAttack:
      character.addTemporaryStats(-statAmount, 0);
      break;
    case PotionType::WoundDefense:
      character.addTemporaryStats(0, -statAmount);
      break;
  }
}

bool Potion::onUse(Entity& usingEntity) {
  Character* character = dynamic_cast<Character*>(&usingEntity);
  if (!character || !isPlayer(character->raceType())) return false;
  use(*character);
  return true;
}
