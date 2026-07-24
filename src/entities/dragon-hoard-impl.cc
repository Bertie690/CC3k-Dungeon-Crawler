module dragonhoard;

#ifdef __INTELLISENSE__
#include "dragon-hoard.cc"
#include "item.cc"
#include "../enums/race-type.cc"
#else
import goldsize;
import item;
import racetype;
#endif  // __INTELLISENSE__

using namespace std;
DragonHoard::DragonHoard(Position position, const shared_ptr<Enemy>& dragon) : GoldPile{position, GoldSize::DragonHoard}, dragon{dragon} {
  dragon->attach(this);
}

DragonHoard::~DragonHoard() {
  if (dragon) {
    dragon->detach(this);
  }
}

bool DragonHoard::canPickup(Character& character) const {
  return isPlayer(character.raceType()) && !guarded;
}

OverlapResult DragonHoard::onOverlap(Entity& movingEntity) {
  if (guarded && isPlayer(dynamic_cast<Character*>(&movingEntity)->raceType())) {
    return OverlapResult::Enter;
  }
  // Determine behaviour like any other Item depending on movingEntity
  return Item::onOverlap(movingEntity);
}

void DragonHoard::onNotify(const EntityDeathEvent&) { guarded = false; }
