export module character:decorators;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include "character-character.cc"
#include "../floor/position.cc"
#include "../floor/floor.cc"
#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#else
import <memory>;
import position;
import floor;
import :character;
import action;
import goldsize;
import entity;
import stats;
import racetype;
#endif // __INTELLISENSE__

class CharacterDecorator : public Character {
  std::unique_ptr<Character> character;

 public:
  // Return whether this Character is dead.
  virtual bool isDead() const override final;

  virtual Action getNextMove(Floor& floor) override final;

  // Return this Character's current stats.
  virtual Stats getStats() const override;
  // Return this Character's current accuracy multiplier against the given defender.
  virtual double getAccuracy(const Character& defender) const override;
  // Return this Character's current evasion multiplier against the given attacker.
  // (For example, a value of 2 translates to a 50% decrease to enemy accuracy.)
  virtual double getEvasion(const Character& attacker) const override;
  // Return the number of attacks this Character can make per action taken.
  // (They will all strike the same target.)
  virtual unsigned int getAttacksPerTurn() const override;
  // Return the damage multiplier this Character has against the given defender.
  virtual double getAttackDamageMultiplier(const Character& defender) const override;
  // Return the amount of gold this Character will drop when killed.
  virtual GoldSize getGoldDrop() const override;
  // Return the multiplier this Character places on opposing HP-draining effects.
  // Negative amounts will result in the attacker taking damage instead of healing.
  virtual double getDrainMulti(const Character& attacker) const override;
  // Trigger effects when this Character lands a hit on another Character.
  virtual void onHit(Character& defender, unsigned int damage) override;
  // Trigger effects when this Character takes a hit from another Character.
  virtual void onBeingAttacked(Character& attacker, unsigned int damage) override;

 public:

 CharacterDecorator(std::unique_ptr<Character> character)
      : character(std::move(character)) {}

  virtual ~CharacterDecorator() = 0;

  virtual RaceType raceType() const override;

  // Deal the given amount of damage to this Character.
  // If lethal is set to false, the damage dealt will not reduce this Character below 1 HP.
  virtual void damage(unsigned int amt, bool lethal = true) override final;
  // Heal this Character up to its maximum HP.
  virtual void heal(unsigned int amt) override final;

  // Return the multiplier for the potency of this Character's potions.
  virtual double getPotionEffectMultiplier() const override;
  // Return the multiplier for this Character's score. Unused for enemies (for now...)
  virtual double getScoreMulti() const override;

  // Return whether this Character can attack a defender of the given type.
  // Intended to be hooked into during command selection, and is entirely unused for player-controlled characters.
  virtual bool canAttack(const RaceType& defenderType) const override;
};

CharacterDecorator::~CharacterDecorator() = default;

bool CharacterDecorator::isDead() const { return character->isDead(); }

Action CharacterDecorator::getNextMove(Floor& floor) {
  return character->getNextMove(floor);
}

Stats CharacterDecorator::getStats() const { return character->getStats(); }

double CharacterDecorator::getAccuracy(const Character& defender) const {
  return character->getAccuracy(defender);
}

double CharacterDecorator::getEvasion(const Character& attacker) const {
  return character->getEvasion(attacker);
}

unsigned int CharacterDecorator::getAttacksPerTurn() const {
  return character->getAttacksPerTurn();
}

double CharacterDecorator::getAttackDamageMultiplier(const Character& defender) const {
  return character->getAttackDamageMultiplier(defender);
}

GoldSize CharacterDecorator::getGoldDrop() const { return character->getGoldDrop(); }

double CharacterDecorator::getDrainMulti(const Character& attacker) const {
  return character->getDrainMulti(attacker);
}

void CharacterDecorator::onHit(Character& defender, unsigned int damage) {
  character->onHit(defender, damage);
}

void CharacterDecorator::onBeingAttacked(Character& attacker, unsigned int damage) {
  character->onBeingAttacked(attacker, damage);
}

RaceType CharacterDecorator::raceType() const { return character->raceType(); }

void CharacterDecorator::damage(unsigned int amt, bool lethal) {
  character->damage(amt, lethal);
}

void CharacterDecorator::heal(unsigned int amt) { character->heal(amt); }

double CharacterDecorator::getPotionEffectMultiplier() const {
  return character->getPotionEffectMultiplier();
}

double CharacterDecorator::getScoreMulti() const {
  return character->getScoreMulti();
}

bool CharacterDecorator::canAttack(const RaceType& defenderType) const {
  return character->canAttack(defenderType);
}