module character;

#ifdef __INTELLISENSE__
#include <cmath>
#include <memory>
#include <variant>

#include "character.cc"
#else
import <cmath>;
import <memory>;
import <variant>;
import floor;
import cell;
import action;
#endif  // __INTELLISENSE__

unsigned int Character::atk() const { return this->getStats().atk; }
unsigned int Character::def() const { return this->getStats().def; }
unsigned int Character::maxHp() const { return this->getStats().maxHp; }
Stats Character::stats() const { return this->getStats(); }
bool Character::dead() const { return this->isDead(); }

void Character::attack(Character& defender, Floor& floor) {
  const unsigned int attacks = this->getAttacksPerTurn();
  for (unsigned int i = 0; i < attacks; i++) {
    const double accuracy = this->getAccuracy(defender);
    const double evasion = defender.getEvasion(*this);

    if (floor.rng.randDouble() > (accuracy / evasion)) {
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
    if (defender.dead() && !isPlayer(defender.raceType())) {
      floor.remove(defender);
      return;
    }
  }
}

void Character::act(Floor& floor) {
  Action nextMove = this->getNextMove(floor);

  if (Pass* pass = std::get_if<Pass>(&nextMove)) {
    return;
  }

  if (Move* move = std::get_if<Move>(&nextMove)) {
    floor.move(*this, this->position() + move->dir);
    return;
  }

  if (UsePotion* potion = std::get_if<UsePotion>(&nextMove)) {
    // TODO: Implement potion usage
    return;
  }

  Attack attack = std::get<Attack>(nextMove);
  const Cell& c = floor.getCell(this->position() + attack.dir);

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

  this->attack(*character, floor);
}

void Character::drain(Character& defender, unsigned int hp) {
  defender.damage(hp);
  if (const double mult = defender.getDrainMulti(*this); mult > 1.0) {
    this->heal(hp * mult);
  } else {
    this->damage(hp * static_cast<int>(-mult));
  }
}
