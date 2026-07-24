module character;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../enums/race-type.cc"
#include "../floor/floor.cc"
#include "../floor/position.cc"
#include "character.cc"
#else
import <memory>;
import position;
import floor;
import action;
import goldsize;
import entity;
import stats;
import racetype;
#endif  // __INTELLISENSE__

CharacterDecorator::CharacterDecorator(std::unique_ptr<Character> character)
    : character(std::move(character)) {}
#pragma region Method Forwards
CharacterDecorator::~CharacterDecorator() = default;

Position& CharacterDecorator::position() noexcept { return character->position(); }
const Position& CharacterDecorator::position() const noexcept { return character->position(); }

bool CharacterDecorator::isDead() const { return character->isDead(); }

Action CharacterDecorator::getNextMove(Floor& floor) { return character->getNextMove(floor); }

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
int CharacterDecorator::getExtraGoldDrop() const { return character->getExtraGoldDrop(); }

double CharacterDecorator::getDrainMulti(const Character& attacker) const {
  return character->getDrainMulti(attacker);
}

void CharacterDecorator::onHit(Character& defender, unsigned int damage) {
  character->onHit(defender, damage);
}

void CharacterDecorator::onBeingAttacked(Character& attacker, unsigned int damage) {
  character->onBeingAttacked(attacker, damage);
}
void CharacterDecorator::onDeath(Character* killer) { character->onDeath(killer); }
void CharacterDecorator::onTurnEnd() { character->onTurnEnd(); }

RaceType CharacterDecorator::raceType() const { return character->raceType(); }

void CharacterDecorator::damage(unsigned int amt, bool lethal, Character* source) { character->damage(amt, lethal, source); }

void CharacterDecorator::heal(unsigned int amt) { character->heal(amt); }

double CharacterDecorator::getPotionEffectMultiplier() const {
  return character->getPotionEffectMultiplier();
}

double CharacterDecorator::getScoreMulti() const { return character->getScoreMulti(); }

bool CharacterDecorator::canAttack(const RaceType& defenderType) const {
  return character->canAttack(defenderType);
}
#pragma endregion  // Method Forwards

#pragma region TempCharacterDecorator
TempCharacterDecorator::TempCharacterDecorator(std::unique_ptr<Character> character,
                                               std::unique_ptr<Character>* ownerSlot)
    : CharacterDecorator(std::move(character)), prev(ownerSlot) {}

// destructor just deletes next ptr like normal
TempCharacterDecorator::~TempCharacterDecorator() = default;

void TempCharacterDecorator::unlink() {
  if (unlinked || !prev) return;

  unlinked = true;

  // this is basically like setting *prev = next; in a doubly-linked list, but with a bit more work to avoid leakage

  auto oldPrev = std::move(*prev);

  auto tmp = std::move(this->character);
  *prev = std::move(tmp);

  prev = nullptr;
}
void TempCharacterDecorator::onNotify(const NewFloorEvent&) { this->unlink(); }
#pragma endregion TempCharacterDecorator
