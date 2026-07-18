module enemy;

#ifdef __INTELLISENSE__
#include "enemy.cc"
#else
import action;
import floor;
#endif  // __INTELLISENSE__

Action Enemy::getNextMove(Floor& floor) {
  // TODO: implement
  return Pass{};
}