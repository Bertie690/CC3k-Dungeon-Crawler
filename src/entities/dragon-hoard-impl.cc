module dragonhoard;

#ifdef __INTELLISENSE__
#include "dragon-hoard.cc"
#include "item.cc"
#include "player.cc"
#else
import goldsize;
import item;
import player;
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
  return dynamic_cast<Player*>(&character) && !guarded;
}

OverlapResult DragonHoard::onOverlap(Entity& movingEntity) {
  if (guarded && dynamic_cast<Player*>(&movingEntity)) {
    return OverlapResult::Enter;
  }
  // Determine behaviour like any other Item depending on movingEntity
  return Item::onOverlap(movingEntity);
}

void DragonHoard::onNotify(const EntityDeathEvent&) { guarded = false; }
