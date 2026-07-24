export module character;

#pragma once

#ifdef __INTELLISENSE__
#include <cmath>
#include <memory>
#include <stdexcept>
#include <variant>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/floor.cc"
#include "../floor/position.cc"
#include "entity.cc"
#include "stats.cc"
#else
import <cmath>;
import <memory>;
import <stdexcept>;
import <variant>;
import action;
import entity;
import floor;
import gameevents;
import goldsize;
import observer;
import position;
import racetype;
import stats;
#endif  // __INTELLISENSE__

// Enum class representing the different strategies a Character can use to determine its next move.
// Used to allow sharing different strategies across multiple instances.
export enum class CharacterMoveStrategyType {
  PlayerInput,
  Random,
  Dragon,
};

export class CharacterDecorator;  // for friendship

// A Character represents an Entity that can move and partake in combat.
export class Character : public virtual Entity {
  friend class
      CharacterDecorator;  // Allow decorators to access protected methods to allow cross-instance access

 protected:
  // Return this Character's maximum HP.
  unsigned int maxHp() const;
  // Return this Character's attack stat.
  unsigned int atk() const;
  // Return this Character's defense stat.
  unsigned int def() const;

  // Drain the given amount of HP from the defender, healing this Character for the same amount.
  void drain(Character& defender, unsigned int hp);

  // Return whether this Character is dead.
  virtual bool isDead() const = 0;

  // Attempt to attack another Character.
  void attack(Character& defender, Floor& floor);

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
  // Return the size of the pile of Gold this Character will drop when killed.
  virtual GoldSize getGoldDrop() const = 0;
  // Return the amount of extra gold this Character will obtain when killing an enemy.
  // Unused by enemies (for now), but can be used to implement special player abilities.
  virtual int getExtraGoldDrop() const = 0;
  // Return the multiplier this Character places on opposing HP-draining effects.
  // Negative amounts will result in the attacker taking damage instead of healing.
  virtual double getDrainMulti(const Character& attacker) const = 0;
  // Trigger effects when this Character lands a hit on another Character.
  virtual void onHit(Character& defender, unsigned int damage) = 0;
  // Trigger effects when this Character takes a hit from another Character.
  virtual void onBeingAttacked(Character& attacker, unsigned int damage) = 0;
  // Trigger effects when this Character is killed, right before it is removed from the Floor.
  // The killer parameter may be null if the Character was killed by a non-Character source (e.g. a potion).
  virtual void onDeath(Character* killer) = 0;
  virtual void onTurnEnd() = 0;

 public:
  virtual ~Character() = default;

  virtual RaceType raceType() const = 0;

  // Player fields for decorators
  // Expose the Player's strategy so the input handler can use it as an observer.
  virtual Observer<PlayerActionEvent>* inputObserver();
  // Return Player's gold.
  virtual int getGold() const;
  // Add amount to Player's gold.
  virtual void addGold(int amount);

  // Public read-only values used by renderers and other presentation code.
  virtual unsigned int currentHp() const = 0;
  Stats stats() const;
  bool dead() const;

  // Perform an action for the turn.
  void act(Floor& floor);

  // Deal the given amount of damage to this Character.
  // If lethal is set to false, the damage dealt will not reduce this Character below 1 HP.
  // The source parameter indicates the Character dealing the damage, if any (for the purpose of on-kill procs).
  virtual void damage(unsigned int amt, bool lethal = true, Character* source = nullptr) = 0;
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

#pragma region Movement Strategies

// A CharacterMoveStrategy defines how a Character will choose its next move.
export class CharacterMoveStrategy {
  friend class BaseCharacter;

  // Return the next Action this Character should take, given the current state of the Floor.
  virtual Action getNextMove(Floor& floor, Character& character) = 0;

 public:
  virtual ~CharacterMoveStrategy() = default;
};

export class PlayerInputMoveStrategy final : public CharacterMoveStrategy,
                                             public Observer<PlayerActionEvent> {
  // The input handler's determined next action.
  Action nextAction{Pass{}};

  virtual void onNotify(const PlayerActionEvent& event) override;
  virtual Action getNextMove(Floor& floor, Character& character) override;
};

class RandomMoveStrategy final : public CharacterMoveStrategy {
  virtual Action getNextMove(Floor& floor, Character& character) override;

 public:
  static inline bool frozen = false;
};

export class DragonMoveStrategy final : public CharacterMoveStrategy {
  virtual Action getNextMove(Floor& floor, Character& character) override;

  const Position goldPos = Position{-1, -1};

 public:
  DragonMoveStrategy(const Position& goldPos);
};

// Toggle whether enemies are frozen or not.
// Defaults to toggling the current state, but can be set explicitly on/off if needed.
export void freezeEnemies(bool frozen = !RandomMoveStrategy::frozen) {
  RandomMoveStrategy::frozen = frozen;
}

#pragma endregion Movement Strategies

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
  virtual unsigned int currentHp() const override final;

  virtual double getPotionEffectMultiplier() const override;
  virtual double getScoreMulti() const override;

  virtual bool canAttack(const RaceType& defender) const override;
};

// Abstract base decorator class for Characters, allowing for temporary or permanent modifications to their behavior.
class CharacterDecorator : public Character {
 protected:
  std::unique_ptr<Character> character;
  virtual bool isDead() const override final;

  virtual Action getNextMove(Floor& floor) override final;

  virtual Stats getStats() const override;
  virtual double getAccuracy(const Character& defender) const override;
  virtual double getEvasion(const Character& attacker) const override;
  virtual unsigned int getAttacksPerTurn() const override;
  virtual double getAttackDamageMultiplier(const Character& defender) const override;
  virtual GoldSize getGoldDrop() const override final;
  virtual int getExtraGoldDrop() const override;
  virtual double getDrainMulti(const Character& attacker) const override;
  virtual void onHit(Character& defender, unsigned int damage) override;
  virtual void onBeingAttacked(Character& attacker, unsigned int damage) override;

  virtual void onDeath(Character* killer) override;
  virtual void onTurnEnd() override;

 public:
  CharacterDecorator(std::unique_ptr<Character> character);
  virtual ~CharacterDecorator() = 0;

  virtual Position& position() noexcept override;
  virtual const Position& position() const noexcept override;

  virtual RaceType raceType() const override final;
  virtual Observer<PlayerActionEvent>* inputObserver() override;
  virtual int getGold() const override;
  virtual void addGold(int amount) override;

  virtual void damage(unsigned int amt, bool lethal = true, Character* source = nullptr) override final;
  virtual void heal(unsigned int amt) override final;
  virtual unsigned int currentHp() const override;

  virtual double getPotionEffectMultiplier() const override;
  virtual double getScoreMulti() const override;

  virtual bool canAttack(const RaceType& defenderType) const override final;
};

// Class for temporary decorators that are automatically removed when a new floor is generated.
class TempCharacterDecorator : public CharacterDecorator, public Observer<NewFloorEvent> {
  std::unique_ptr<Character>* prev = nullptr;
  bool unlinked = false;

  void unlink();

  virtual void onNotify(const NewFloorEvent&) override final;

 public:
  TempCharacterDecorator(std::unique_ptr<Character> character,
                         std::unique_ptr<Character>* ownerSlot);
  virtual ~TempCharacterDecorator() = 0;
};

#pragma region Concrete Decorators

export class PotionEffectCharacterDecorator final : public CharacterDecorator {
  const double effectMulti;

 protected:
  virtual double getPotionEffectMultiplier() const override {
    return CharacterDecorator::getPotionEffectMultiplier() * effectMulti;
  }

 public:
  PotionEffectCharacterDecorator(std::unique_ptr<Character> character, double effectMulti)
      : CharacterDecorator(std::move(character)), effectMulti(effectMulti) {}
  virtual ~PotionEffectCharacterDecorator() = default;
};

export class AttacksPerTurnCharacterDecorator final : public CharacterDecorator {
  const unsigned int attacks;

 protected:
  virtual unsigned int getAttacksPerTurn() const override {
    return CharacterDecorator::getAttacksPerTurn() + attacks;
  }

 public:
  AttacksPerTurnCharacterDecorator(std::unique_ptr<Character> character, unsigned int attacks)
      : CharacterDecorator(std::move(character)), attacks(attacks) {}
  virtual ~AttacksPerTurnCharacterDecorator() = default;
};

export class HpDrainCharacterDecorator final : public CharacterDecorator {
  // The amount of hit-points to drain.
  const unsigned int hpDrain;

 protected:
  virtual void onHit(Character& defender, unsigned int damage) override {
    CharacterDecorator::onHit(defender, damage);
    this->drain(defender, hpDrain);
  }

 public:
  HpDrainCharacterDecorator(std::unique_ptr<Character> character, unsigned int hpDrain)
      : CharacterDecorator(std::move(character)), hpDrain(hpDrain) {}
  virtual ~HpDrainCharacterDecorator() = default;
};

export class ScoreMultiCharacterDecorator final : public CharacterDecorator {
  const double scoreMulti;

 protected:
  virtual double getScoreMulti() const override {
    return CharacterDecorator::getScoreMulti() * scoreMulti;
  }

 public:
  ScoreMultiCharacterDecorator(std::unique_ptr<Character> character, double scoreMulti)
      : CharacterDecorator(std::move(character)), scoreMulti(scoreMulti) {}
  virtual ~ScoreMultiCharacterDecorator() = default;
};

export class RaceTypeDamageMultiplierCharacterDecorator final : public CharacterDecorator {
  const RaceType targetRace;
  const double damageMulti;

 protected:
  virtual double getAttackDamageMultiplier(const Character& defender) const override {
    return CharacterDecorator::getAttackDamageMultiplier(defender) *
    (defender.raceType() == targetRace ? damageMulti : 1.0);
  }

  public:
  RaceTypeDamageMultiplierCharacterDecorator(std::unique_ptr<Character> character,
                                             RaceType targetRace,
                                             double damageMulti)
      : CharacterDecorator(std::move(character)), targetRace(targetRace), damageMulti(damageMulti) {}
  virtual ~RaceTypeDamageMultiplierCharacterDecorator() = default;
};

export class TurnHpRegenCharacterDecorator final : public CharacterDecorator {
  const unsigned int hpRegen;

 protected:
  virtual void onTurnEnd() override {
    CharacterDecorator::onTurnEnd();
    this->heal(hpRegen);
  }

 public:
  TurnHpRegenCharacterDecorator(std::unique_ptr<Character> character, unsigned int hpRegen)
      : CharacterDecorator(std::move(character)), hpRegen(hpRegen) {}
  virtual ~TurnHpRegenCharacterDecorator() = default;
};

export class GoldOnKillCharacterDecorator final : public CharacterDecorator {
  const int goldOnKill;

 protected:
  virtual int getExtraGoldDrop() const override {
    return CharacterDecorator::getExtraGoldDrop() + goldOnKill;
  }

 public:
  GoldOnKillCharacterDecorator(std::unique_ptr<Character> character, int goldOnKill)
      : CharacterDecorator(std::move(character)), goldOnKill(goldOnKill) {}
  virtual ~GoldOnKillCharacterDecorator() = default;
};

export class ReverseDrainCharacterDecorator final : public CharacterDecorator {
 protected:
  virtual double getDrainMulti(const Character& attacker) const override {
    return -CharacterDecorator::getDrainMulti(attacker);
  }

 public:
  ReverseDrainCharacterDecorator(std::unique_ptr<Character> character)
      : CharacterDecorator(std::move(character)) {}
  virtual ~ReverseDrainCharacterDecorator() = default;
};

export class DodgeChanceCharacterDecorator final : public CharacterDecorator {
  const double dodgeChance;

 protected:
  virtual double getEvasion(const Character& attacker) const override {
    return CharacterDecorator::getEvasion(attacker) / (1.0 - dodgeChance);
  }

 public:
  DodgeChanceCharacterDecorator(std::unique_ptr<Character> character, double dodgeChance)
      : CharacterDecorator(std::move(character)), dodgeChance(dodgeChance) {}
  virtual ~DodgeChanceCharacterDecorator() = default;
};

export class TempStatChangeCharacterDecorator final : public TempCharacterDecorator {
  const Stats statDelta;
  const bool increase;

 protected:
  virtual Stats getStats() const override {
    return increase ?
      CharacterDecorator::getStats() + statDelta
      : CharacterDecorator::getStats() - statDelta;
  }

 public:
  TempStatChangeCharacterDecorator(std::unique_ptr<Character> character,
                                   std::unique_ptr<Character>* ownerSlot,
                                   Stats statDelta,
                                   bool increase = true)
      : TempCharacterDecorator(std::move(character), ownerSlot), statDelta(statDelta), increase(increase) {}
  virtual ~TempStatChangeCharacterDecorator() = default;
};



#pragma endregion  // Concrete Decorators
