export module merchant;

#ifdef __INTELLISENSE__
#include "../enums/race-type.cc"
#include "../enums/gold-size.cc"
#include "enemy.cc"
#else
import racetype;
import goldsize;
import enemy;
#endif  // __INTELLISENSE__

export class Merchant : public Enemy {
  inline static bool hostile = false;

  virtual bool canAttack(const RaceType& defenderType) const override {
    return hostile && Enemy::canAttack(defenderType);
  }

  virtual void onBeingAttacked(Character&, unsigned int) override { hostile = true; }
  virtual GoldDrop getGoldDrop() const override {
    return NormalGoldDrop{.pileSize = GoldSize::MerchantHoard, .pilesDropped = 1};
  }

 public:
  using Enemy::Enemy;
};
