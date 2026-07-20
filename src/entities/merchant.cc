export module merchant;

#ifdef __INTELLISENSE__
#include "enemy.cc"
#else
import enemy;
#endif  // __INTELLISENSE__

export class Merchant : public Enemy {
  static bool hostile;

  virtual bool canAttack(const RaceType& defenderType) const override {
    return hostile && Enemy::canAttack(defenderType);
  }

  virtual void onBeingAttacked(Character&, unsigned int) override { hostile = true; }

 public:
  using Enemy::Enemy;
};