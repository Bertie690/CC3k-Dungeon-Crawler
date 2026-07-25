module enemy;

#ifdef __INTELLISENSE__
#include "enemy.cc"
#endif  // __INTELLISENSE__

using namespace std;

double Enemy::getAccuracy(const Character& defender) const { return 0.5; }

void Enemy::onDeath(Character* killer) {
  BaseCharacter::onDeath(killer);
  notify(EntityDeathEvent{position()});
}
