export module character;

#pragma once

#ifdef __INTELLISENSE__
#include <cmath>
#include <stdexcept>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../floor/position.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "entity.cc"
#include "stats.cc"
#else
import action;
import goldsize;
import cell;
import floor;
import entity;
import stats;
import racetype;
import position;
import <stdexcept>;
import <cmath>;
#endif  // __INTELLISENSE__

// A Character represents an Entity that can move and partake in combat.
export class Character : public Entity {
  // This Character's current HP.
  int hp;
  // The character's base stats.
  const Stats baseStats;

  // Return the next move this Character will take.
  virtual Action getNextMove() = 0;

  // Return this Character's maximum HP.
  unsigned int maxHp() const;
  // Return this Character's attack stat.
  unsigned int atk() const;
  // Return this Character's defense stat.
  unsigned int def() const;

  // Return whether this Character is dead.
  bool isDead() const;

  // Attempt to attack another Character.
  void attack(Character& defender);

 protected:
  // Get this Character's current position.
  const Position& pos() const;

  // Return a reference to the current floor.
  const Floor& getFloor() const;

  // Return this Character's current stats.
  virtual Stats getStats() const;
  // Return this Character's current accuracy multiplier against the given defender.
  virtual double getAccuracy(Character& defender) const;
  // Return this Character's current evasion multiplier against the given attacker.
  virtual double getEvasion(Character& attacker) const;
  // Return the number of attacks this Character can make per action taken.
  // (They will all strike the same target.)
  virtual unsigned int getAttacksPerTurn() const;
  // Return the damage multiplier this Character has against the given defender.
  virtual double getAttackDamageMultiplier(Character& defender) const;
  // Return the amount of gold this Character will drop when killed.
  virtual GoldSize getGoldDrop() const;
  // Return the multiplier this Character places on opposing HP-draining effects.
  // Negative amounts will result in the attacker taking damage instead of healing.
  virtual double getDrainMulti(Character& attacker) const;
  // Trigger effects when this Character lands a hit on another Character.
  virtual void onHit(Character& defender, unsigned int damage);
  // Trigger effects when this Character takes a hit from another Character.
  virtual void onBeingAttacked(Character& attacker, unsigned int damage);

  // Attempt to drain the given amount of HP from the defender.
  void drain(Character& defender, int amt);

 public:
  // The race this character belongs to, for identification & display purposes.
  const RaceType raceType;

  Character(Position position, Stats baseStats, RaceType raceType);
  virtual ~Character() = default;

  // Perform an action for the turn.
  void act(Floor& floor);
  // Deal the given amount of damage to this Character.
  // If lethal is set to false, the damage dealt will not reduce this Character below 1 HP.
  void damage(unsigned int amt, bool lethal = true);
  // Heal this Character up to its maximum HP.
  void heal(unsigned int amt);

  // Return the multiplier for the potency of this Character's potions.
  virtual double getPotionEffectMultiplier() const;
  // Return the multiplier for this Character's score. Unused for enemies (for now...)
  virtual double getScoreMulti() const;
};