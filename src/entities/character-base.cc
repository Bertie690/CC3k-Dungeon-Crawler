export module character:base;

#pragma once

#ifdef __INTELLISENSE__
#include <stdexcept>
#include <variant>

#include "../enums/race-type.cc"
#include "character-character.cc"
#include "character-move-strategy-type.cc"
#include "character-movement.cc"
#else
import <variant>;
import racetype;
import position;
import stats;
import :character;
import :movestrategytype;
import :movement;
#endif

// Base class that implements the Character interface, providing default implementations for most of its methods.
// Still technically virtual, but can be instantiated directly (unlike Character).
export class BaseCharacter : public BaseEntity, public Character {
  // This Character's current HP.
  int hp;
  // The character's base stats.
  const Stats baseStats;
  // The race this character belongs to, for identification & display purposes.
  const RaceType type;

  // The type of movement strategy this Character employs.
  // A new copy is created upon class creation.
  std::unique_ptr<CharacterMoveStrategy> moveStrategy;

  virtual bool isDead() const override final;

 protected:
  virtual Stats getStats() const override final;

  virtual Action getNextMove(Floor& floor) override final;

  CharacterMoveStrategy& movementStrategy();

  virtual double getAccuracy(const Character& defender) const override;
  virtual double getEvasion(const Character& attacker) const override;
  virtual unsigned int getAttacksPerTurn() const override;
  virtual double getAttackDamageMultiplier(const Character& defender) const override;
  virtual GoldSize getGoldDrop() const override;
  virtual int getExtraGoldDrop() const override;
  virtual double getDrainMulti(const Character& attacker) const override;
  virtual void onHit(Character& defender, unsigned int damage) override;
  virtual void onBeingAttacked(Character& attacker, unsigned int damage) override;
  virtual void onTurnEnd() override;
  virtual void onDeath(Character* killer) override;

 public:
  BaseCharacter(Position position, Stats baseStats, RaceType raceType,
                CharacterMoveStrategyType strategyType);
  BaseCharacter(Position position, Stats baseStats, RaceType raceType,
                std::unique_ptr<CharacterMoveStrategy> strat);
  virtual ~BaseCharacter() = default;

  virtual RaceType raceType() const override final;

  virtual void damage(unsigned int amt, bool lethal = true, Character* source = nullptr) override final;
  virtual void heal(unsigned int amt) override final;

  virtual double getPotionEffectMultiplier() const override;
  virtual double getScoreMulti() const override;

  virtual bool canAttack(const RaceType& defender) const override;
};
