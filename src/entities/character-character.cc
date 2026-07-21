export module character:character;

#pragma once

#ifdef __INTELLISENSE__
#include <cmath>
#include <memory>
#include <stdexcept>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../floor/floor.cc"
#include "character-decorators.cc"
#include "character-movement.cc"
#include "entity.cc"
#include "stats.cc"
#else
import action;
import goldsize;
import floor;
import entity;
import stats;
import racetype;
import <stdexcept>;
import <cmath>;
import <memory>;
#endif  // __INTELLISENSE__

// A Character represents an Entity that can move and partake in combat.
export class Character : public virtual Entity {
 protected:

  // Return this Character's maximum HP.
  virtual unsigned int maxHp() const = 0;
  // Return this Character's attack stat.
  virtual unsigned int atk() const = 0;
  // Return this Character's defense stat.
  virtual unsigned int def() const = 0;

  // Return whether this Character is dead.
  virtual bool isDead() const = 0;

  // Attempt to attack another Character.
  virtual void attack(Character& defender, Floor& floor);

  virtual Action getNextMove(Floor& floor) = 0;

  // Return this Character's current stats.
  virtual Stats getStats() const = 0;
  // Return this Character's current accuracy multiplier against the given defender.
  virtual double getAccuracy(const Character& defender) const = 0;
  // Return this Character's current evasion multiplier against the given attacker.
  // (For example, a value of 2 translates to a 50% decrease to enemy accuracy.)
  virtual double getEvasion(const Character& attacker) const = 0;
  // Return the number of attacks this Character can make per action taken.
  // (They will all strike the same target.)
  virtual unsigned int getAttacksPerTurn() const = 0;
  // Return the damage multiplier this Character has against the given defender.
  virtual double getAttackDamageMultiplier(const Character& defender) const = 0;
  // Return the amount of gold this Character will drop when killed.
  virtual GoldSize getGoldDrop() const = 0;
  // Return the multiplier this Character places on opposing HP-draining effects.
  // Negative amounts will result in the attacker taking damage instead of healing.
  virtual double getDrainMulti(const Character& attacker) const = 0;
  // Trigger effects when this Character lands a hit on another Character.
  virtual void onHit(Character& defender, unsigned int damage) = 0;
  // Trigger effects when this Character takes a hit from another Character.
  virtual void onBeingAttacked(Character& attacker, unsigned int damage) = 0;

 public:
  virtual ~Character() = default;

  virtual RaceType raceType() const = 0;

  // Perform an action for the turn.
  virtual void act(Floor& floor);

  // Deal the given amount of damage to this Character.
  // If lethal is set to false, the damage dealt will not reduce this Character below 1 HP.
  virtual void damage(unsigned int amt, bool lethal = true) = 0;
  // Heal this Character up to its maximum HP.
  virtual void heal(unsigned int amt) = 0;

  // Return the multiplier for the potency of this Character's potions.
  virtual double getPotionEffectMultiplier() const = 0;
  // Return the multiplier for this Character's score. Unused for enemies (for now...)
  virtual double getScoreMulti() const = 0;

  // Return whether this Character can attack a defender of the given type.
  // Intended to be hooked into during command selection, and is entirely unused for player-controlled characters.
  virtual bool canAttack(const RaceType& defenderType) const = 0;
};
