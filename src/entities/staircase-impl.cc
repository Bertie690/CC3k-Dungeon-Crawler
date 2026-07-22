module staircase;

#ifdef __INTELLISENSE__
#include "player.cc"
#include "staircase.cc"
#else
import player;
#endif  // __INTELLISENSE__

OverlapResult Staircase::onOverlap(Entity& movingEntity) {
  if (dynamic_cast<Player*>(&movingEntity)) {
    return OverlapResult::FloorTransition;
  }
  return OverlapResult::Blocked;
}
