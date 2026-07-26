module character;

#ifdef __INTELLISENSE__
#include <cmath>
#include <stdexcept>
#include <variant>

#include "../enums/gold-size.cc"
#include "character.cc"
#else
import <cmath>;
import <stdexcept>;
import <variant>;
import goldsize;
#endif  // __INTELLISENSE__

// Helper function to instantiate a movement strategy based on the corresponding enum value.
std::unique_ptr<CharacterMoveStrategy> buildStrategy(CharacterMoveStrategyType type) {
  switch (type) {
    case CharacterMoveStrategyType::PlayerInput:
      return std::make_unique<PlayerInputMoveStrategy>();
    case CharacterMoveStrategyType::Random:
      return std::make_unique<RandomMoveStrategy>();
    case CharacterMoveStrategyType::Dragon:
      throw std::invalid_argument(
          "DragonMoveStrategy requires a gold position to be specified... please construct "
          "directly");
  }
  throw std::invalid_argument("CharacterMoveStrategyType lacks strategy");
}

BaseCharacter::BaseCharacter(Position position, Stats baseStats, RaceType raceType,
                             CharacterMoveStrategyType strategyType)
    : BaseEntity{position},
      hp(baseStats.maxHp),
      baseStats(baseStats),
      type(raceType),
      moveStrategy(buildStrategy(strategyType)) {};
BaseCharacter::BaseCharacter(Position position, Stats baseStats, RaceType raceType,
                             std::unique_ptr<CharacterMoveStrategy> strat)
    : BaseEntity{position},
      hp(baseStats.maxHp),
      baseStats(baseStats),
      type(raceType),
      moveStrategy(std::move(strat)) {};

#pragma region Getters/Hooks
bool BaseCharacter::isDead() const { return this->hp <= 0U; }
unsigned int BaseCharacter::currentHp() const { return std::max(0U, this->hp); }

Stats BaseCharacter::getStats() const {
  Stats result = this->baseStats;
  result.atk = static_cast<unsigned int>(std::max(0, static_cast<int>(result.atk) + temporaryAtk));
  result.def = static_cast<unsigned int>(std::max(0, static_cast<int>(result.def) + temporaryDef));
  return result;
}
double BaseCharacter::getAccuracy(const Character& defender) const { return 1.0; }
double BaseCharacter::getEvasion(const Character& attacker) const { return 1.0; }
unsigned int BaseCharacter::getAttacksPerTurn() const { return 1; }
double BaseCharacter::getAttackDamageMultiplier(const Character& defender) const { return 1.0; }
GoldDrop BaseCharacter::getGoldDrop() const {
  return GoldDrop{.instantAmount = static_cast<unsigned int>(GoldSize::Small)};
}
double BaseCharacter::getDrainMulti(const Character& attacker) const { return 1.0; }
void BaseCharacter::onHit(Character& defender, unsigned int damage) {}
void BaseCharacter::onBeingAttacked(Character& attacker, unsigned int damage) {}
void BaseCharacter::onDeath(Character* killer) { Character::onDeath(killer); }
void BaseCharacter::onKill(const Character& killer) { Character::onKill(killer);}
void BaseCharacter::onTurnEnd() {}

double BaseCharacter::getPotionEffectMultiplier() const { return 1.0; }
double BaseCharacter::getScoreMulti() const { return 1.0; }
bool BaseCharacter::canAttack(const RaceType& defenderType) const {
  return isPlayer(this->raceType()) != isPlayer(defenderType);
}
void BaseCharacter::addTemporaryStats(int atkDelta, int defDelta) {
  temporaryAtk += atkDelta;
  temporaryDef += defDelta;
}
void BaseCharacter::resetTemporaryStats() {
  temporaryAtk = 0;
  temporaryDef = 0;
}
RaceType BaseCharacter::raceType() const { return this->type; }
CharacterMoveStrategy& BaseCharacter::movementStrategy() { return *moveStrategy; }

#pragma endregion  // Getters/Hooks

#pragma region Actions

Action BaseCharacter::getNextMove(Floor& floor) {
  return this->moveStrategy->getNextMove(floor, *this);
}

void BaseCharacter::damage(unsigned int amt, bool lethal, Character* source) {
  if (this->isDead()) return;

  if (amt < this->hp) {
    this->hp -= amt;
    return;
  }
  if (lethal) {
    this->hp = 0;
    this->onDeath(source);
  } else {
    this->hp = 1;
  }
}

void BaseCharacter::heal(unsigned int amt) {
  if (this->isDead()) {
    throw std::runtime_error("Cannot heal a dead character.");
  }
  this->hp = std::min(this->maxHp(), this->hp + amt);
}
