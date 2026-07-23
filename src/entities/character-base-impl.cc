module character;

#ifdef __INTELLISENSE__
#include <cmath>
#include <stdexcept>
#include <variant>

#include "character-base.cc"
#include "character-movement.cc"
#else
import <cmath>;
import <stdexcept>;
import <variant>;
import :base;
import :movement;
#endif  // __INTELLISENSE__

// Helper function to instantiate a movement strategy based on the corresponding enum value.
std::unique_ptr<CharacterMoveStrategy> buildStrategy(CharacterMoveStrategyType type) {
  switch (type) {
    case CharacterMoveStrategyType::PlayerInput:
      return std::make_unique<PlayerInputMoveStrategy>();
    case CharacterMoveStrategyType::Random:
      return std::make_unique<RandomMoveStrategy>();
    case CharacterMoveStrategyType::Dragon:
      throw std::invalid_argument("DragonMoveStrategy requires a gold position to be specified... please construct directly");
  }
  throw std::invalid_argument("CharacterMoveStrategyType lacks strategy");
}

BaseCharacter::BaseCharacter(Position position, Stats baseStats, RaceType raceType,
                             CharacterMoveStrategyType strategyType)
    : BaseEntity{position},
      baseStats(baseStats),
      type(raceType),
      moveStrategy(buildStrategy(strategyType)) {};
BaseCharacter::BaseCharacter(Position position, Stats baseStats, RaceType raceType,
                             std::unique_ptr<CharacterMoveStrategy> strat)
    : BaseEntity{position},
      baseStats(baseStats),
      type(raceType),
      moveStrategy(std::move(strat)) {};

#pragma region Getters/Hooks
bool BaseCharacter::isDead() const { return this->hp <= 0; }

Stats BaseCharacter::getStats() const { return this->baseStats; }
double BaseCharacter::getAccuracy(const Character& defender) const { return 1.0; }
double BaseCharacter::getEvasion(const Character& attacker) const { return 1.0; }
unsigned int BaseCharacter::getAttacksPerTurn() const { return 1; }
double BaseCharacter::getAttackDamageMultiplier(const Character& defender) const { return 1.0; }
GoldSize BaseCharacter::getGoldDrop() const { return GoldSize::None; }
int BaseCharacter::getExtraGoldDrop() const { return 0; }
double BaseCharacter::getDrainMulti(const Character& attacker) const { return 1.0; }
void BaseCharacter::onHit(Character& defender, unsigned int damage) {}
void BaseCharacter::onBeingAttacked(Character& attacker, unsigned int damage) {}
void BaseCharacter::onDeath(Character* killer) {}
void BaseCharacter::onTurnEnd() {}

double BaseCharacter::getPotionEffectMultiplier() const { return 1.0; }
double BaseCharacter::getScoreMulti() const { return 1.0; }
bool BaseCharacter::canAttack(const RaceType& defenderType) const {
  return isPlayer(this->raceType()) != isPlayer(defenderType);
}
RaceType BaseCharacter::raceType() const { return this->type; }
CharacterMoveStrategy& BaseCharacter::movementStrategy() { return *moveStrategy; }

#pragma endregion  // Getters/Hooks

#pragma region Actions

Action BaseCharacter::getNextMove(Floor& floor) {
  return this->moveStrategy->getNextMove(floor, *this);
}

void BaseCharacter::damage(unsigned int amt, bool lethal, Character* source) {
  this->hp -= amt;
  if (!this->isDead()) {
    return;
  }
  if (lethal) {
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
