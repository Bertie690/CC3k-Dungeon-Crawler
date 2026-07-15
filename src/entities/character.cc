export module character;

#ifdef __INTELLISENSE__
#include "../enums/action.cc"
#include "../enums/potion-type.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "entity.cc"
#include "stats.cc"
#else
import action;
import potiontype;
import cell;
import floor;
import entity;
import stats;
#endif  // __INTELLISENSE__

// A Character represents an Entity that can move and partake in combat.
export class Character : public Entity {
  int hp;
  Cell cell;
  const RaceType race;

  // Get this Character's current position.
  Position pos() const;

  // Get this Character's current stats.
  virtual Stats getStats() const;
  // Get this Character's current accuracy against the given defender.
  virtual double getAccuracy(Character& defender) const;
  // Get this Character's current evasion against the given attacker.
  virtual double getEvasion(Character& attacker) const;
  virtual unsigned int getAttacksPerTurn() const;
  virtual double getAttackDamageMultiplier(Character& defender) const;
  virtual double getGoldMultiplier() const;
  virtual double getPotionEffectMultiplier(PotionType p) const;
  virtual double getDrainMulti(Character& attacker, unsigned int damage) const;

  virtual void onHit(Character& defender, int damage);
  virtual void onDeath();

  virtual Action getNextMove(Floor& f) = 0;

 public:
  virtual ~Character() = default;
  void act(Floor& f, Position pos);
  void damage(int amt, bool lethal = true);
  void heal(int amt);
};