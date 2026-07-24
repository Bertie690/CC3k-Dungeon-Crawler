module item;

#ifdef __INTELLISENSE__
#include "item.cc"
#include "../enums/race-type.cc"
#else
import racetype;
#endif  // __INTELLISENSE__

bool Item::canPickup(Character& character) const {
  return isPlayer(character.raceType());
}

OverlapResult Item::onOverlap(Entity& movingEntity) {
  Character* character = dynamic_cast<Character*>(&movingEntity);
  if (!character || !canPickup(*character)) {
    return OverlapResult::Blocked;
  }

  applyEffect(*character);
  return OverlapResult::Consumed;
}
