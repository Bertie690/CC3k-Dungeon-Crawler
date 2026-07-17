module character;

#ifdef __INTELLISENSE__
#include <cmath>
#include <stdexcept>
#include <variant>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "character.cc"
#include "entity.cc"
#include "stats.cc"
#else
import <variant>;
#endif  // __INTELLISENSE__

#pragma region Getters/Hooks
Character::Character(Stats baseStats, RaceType raceType)
    : Entity{}, baseStats(baseStats), raceType(raceType){};

unsigned int Character::atk() const { return this->getStats().atk; }
unsigned int Character::def() const { return this->getStats().def; }
unsigned int Character::maxHp() const { return this->getStats().maxHp; }
bool Character::isDead() const { return this->hp <= 0; }

Stats Character::getStats() const { return this->baseStats; }
double Character::getAccuracy(Character& defender) const { return 1.0; }
double Character::getEvasion(Character& attacker) const { return 1.0; }
unsigned int Character::getAttacksPerTurn() const { return 1; }
double Character::getAttackDamageMultiplier(Character& defender) const { return 1.0; }
GoldSize Character::getGoldDrop() const { return GoldSize::None; }
double Character::getDrainMulti(Character& attacker) const { return 1.0; }
void Character::onHit(Character& defender, unsigned int damage) {}
void Character::onBeingAttacked(Character& attacker, unsigned int damage) {}

double Character::getPotionEffectMultiplier() const { return 1.0; }
double Character::getScoreMulti() const { return 1.0; }

#pragma endregion  // Getters/Hooks

void Character::drain(Character& defender, int amt) {
  defender.damage(amt);

  const int healing = amt * defender.getDrainMulti(*this);

  if (amt > 0) {
    this->damage(static_cast<unsigned int>(healing));
  } else {
    this->heal(static_cast<unsigned int>(-healing));
  }
}

#pragma region Actions
void Character::act(Floor& floor) {
  Action nextMove = this->getNextMove();

  if (Pass* pass = std::get_if<Pass>(&nextMove)) {
    return;
  }

  if (Move* move = std::get_if<Move>(&nextMove)) {
    // TODO implement movement
  }

  Attack attack = std::get<Attack>(nextMove);

  const Cell& c = this->getFloor().getCell(this->pos() + attack.dir);
  Character* character = nullptr;
  for (const auto& entity : c.getEntities()) {
    character = dynamic_cast<Character*>(entity.get());
    if (character) {
      break;
    }
  }

  if (!character) {
    throw std::invalid_argument("No character to attack in the given direction.");
  }

  this->attack(*character);
}

void Character::attack(Character& defender) {
  const unsigned int attacks = this->getAttacksPerTurn();
  for (unsigned int i = 0; i < attacks; i++) {
    const double accuracy = this->getAccuracy(defender);
    const double evasion = defender.getEvasion(*this);

    if (this->getFloor().rng.randDouble() > accuracy * evasion) {
      // miss
      // TODO: Add a UI event?
      continue;
    }

    const double damageMultiplier = this->getAttackDamageMultiplier(defender);
    // ceiling((100/(100 + Def (Defender))) * Atk(Attacker))
    const unsigned int damage =
        std::ceil(100.0 / (100.0 + defender.def()) * this->atk() * damageMultiplier);
    defender.damage(damage);
    this->onHit(defender, damage);
    defender.onBeingAttacked(*this, damage);
  }
}

void Character::damage(unsigned int amt, bool lethal) {
  this->hp -= amt;
  if (!this->isDead()) {
    return;
  }
  if (lethal) {
    this->hp = 1;
  } else {
    // TODO @ShauryaSuri: implement removal from floor
  }
}

void Character::heal(unsigned int amt) {
  if (this->isDead()) {
    throw std::runtime_error("Cannot heal a dead character.");
  }
  this->hp = std::min(this->maxHp(), this->hp + amt);
}
