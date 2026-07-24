module staircase;

#ifdef __INTELLISENSE__
#include "character.cc"
#include "staircase.cc"
#include "../enums/race-type.cc"
#else
import character;
import racetype;
#endif  // __INTELLISENSE__

OverlapResult Staircase::onOverlap(Entity& movingEntity) {
  Character* character = dynamic_cast<Character*>(&movingEntity);
  if (character && isPlayer(character->raceType())) {
    return OverlapResult::FloorTransition;
  }
  return OverlapResult::Blocked;
}
