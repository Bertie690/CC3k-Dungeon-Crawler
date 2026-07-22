module item;

#ifdef __INTELLISENSE__
#include "item.cc"
#include "player.cc"
#else
import player;
#endif  // __INTELLISENSE__

bool Item::canPickup(Character& character) const {
  return dynamic_cast<Player*>(&character);
}

OverlapResult Item::onOverlap(Entity& movingEntity) {
  Character* character = dynamic_cast<Character*>(&movingEntity);
  if (!character || !canPickup(*character)) {
    return OverlapResult::Blocked;
  }

  applyEffect(*character);
  return OverlapResult::Consumed;
}
