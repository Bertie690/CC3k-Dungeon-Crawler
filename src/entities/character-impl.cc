module character;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <stdexcept>

#include "../enums/action.cc"
#include "../enums/gold-size.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "character.cc"
#include "entity.cc"
#include "stats.cc"
#endif  // __INTELLISENSE__

Character::Character(Stats baseStats, RaceType raceType)
    : Entity{}, baseStats(baseStats), raceType(raceType){};

const Position& Character::pos() const {
  // TODO @ShauryaSuri: implement alongside floor stuff
  static Position p{0, 0};
  return p;
};

const Floor& Character::getFloor() const {
  // TODO @ShauryaSuri: implement alongside floor stuff
  throw 1;
};

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

void Character::drain(Character& defender, int amt) {
  defender.damage(amt);

  const int healing = amt * defender.getDrainMulti(*this);

  if (amt > 0) {
    this->damage(static_cast<unsigned int>(healing));
  } else {
    this->heal(static_cast<unsigned int>(-healing));
  }
}

void Character::act() {
  Action nextMove = this->getNextMove();

  if (Pass* pass = std::get_if<Pass>(&nextMove)) {
    return;
  }
  if (Move* move = std::get_if<Move>(&nextMove)) {
    // TODO implement movement
  }

  Attack attack = std::get<Attack>(nextMove);

  const Cell c = this->getFloor().getCell(this->pos() + attack.dir);
  const auto& character = c.getCharacter();
  if (!character) {
    throw std::invalid_argument("No character to attack in the given direction.");
  }

  // TODO: finish
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
