export module character:decorators;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../events/observer.cc"
#include "../floor/floor.cc"
#include "../floor/position.cc"
#include "character-character.cc"
#else
import <memory>;
import position;
import floor;
import :character;
import action;
import observer;
import goldsize;
import entity;
import stats;
import racetype;
#endif  // __INTELLISENSE__

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

  virtual RaceType raceType() const override final;

  virtual void damage(unsigned int amt, bool lethal = true, Character* source = nullptr) override final;
  virtual void heal(unsigned int amt) override final;

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
    return CharacterDecorator::getEvasion(attacker) * (1.0 + dodgeChance);
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