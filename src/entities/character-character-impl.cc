module character;

#ifdef __INTELLISENSE__
#include <cmath>
#include <memory>
#include <variant>

#include "../events/floor-events.cc"
#include "character.cc"
#else
import <cmath>;
import <memory>;
import <variant>;
import floor;
import cell;
import action;
import floorevents;
#endif  // __INTELLISENSE__

unsigned int Character::atk() const { return this->getStats().atk; }
unsigned int Character::def() const { return this->getStats().def; }
unsigned int Character::maxHp() const { return this->getStats().maxHp; }
Stats Character::stats() const { return this->getStats(); }
bool Character::dead() const { return this->isDead(); }

// defaults for non-player characters
Observer<PlayerActionEvent>* Character::inputObserver() { return nullptr; }
int Character::getGold() const { return 0; }
void Character::addGold(int) {}

void Character::attack(Character& defender, Floor& floor) {
  const unsigned int attacks = this->getAttacksPerTurn();
  for (unsigned int i = 0; i < attacks; i++) {
    const double accuracy = this->getAccuracy(defender);
    const double evasion = defender.getEvasion(*this);

    if (floor.rng.randDouble() > (accuracy / evasion)) {
      floor.reportCharacterAction(
          CharacterAttackEvent{*this, defender, CharacterAttackEvent::Result::Miss});
      continue;
    }

    const double damageMultiplier = this->getAttackDamageMultiplier(defender);
    // ceiling((100/(100 + Def (Defender))) * Atk(Attacker))
    const unsigned int damage =
        std::ceil(100.0 / (100.0 + defender.def()) * this->atk() * damageMultiplier);

    const unsigned int hpBefore = defender.currentHp();
    defender.damage(damage, true, this);
    const unsigned int damageDealt = hpBefore - defender.currentHp();
    this->onHit(defender, damage);
    defender.onBeingAttacked(*this, damage);

    floor.reportCharacterAction(CharacterAttackEvent{
        *this, defender, CharacterAttackEvent::Result::Hit, damageDealt, defender.dead()});

    if (dead()) {
      // vampire atacks dwarf and dies edge case
      floor.reportCharacterDeath(
          CharacterDeathEvent{*this, defender.position(), raceType(), getGoldDrop()});
    }

    if (defender.dead()) {
      // TODO: Push action from onDeath function if possible
      floor.reportCharacterDeath(
          CharacterDeathEvent{defender, position(), defender.raceType(), defender.getGoldDrop()});
      // Defender was removed, so Elves shouldn't attack again
      return;
    }
    // if vampire dies, wait to check if dwarf is dead before returning
    if (dead()) return;
  }
}

void Character::act(Floor& floor) {
  Action nextMove = this->getNextMove(floor);

  if (Pass* pass = std::get_if<Pass>(&nextMove)) {
    this->onTurnEnd();
    return;
  }

  if (Move* move = std::get_if<Move>(&nextMove)) {
    floor.move(*this, this->position() + move->dir);
    this->onTurnEnd();
    return;
  }

  if (UsePotion* potion = std::get_if<UsePotion>(&nextMove)) {
    const Position potionPosition = this->position() + potion->dir;
    if (!floor.hasCell(potionPosition)) {
      throw std::invalid_argument("No potion in the given direction.");
    }
    Cell& cell = floor.getCell(potionPosition);
    for (const auto& entity : cell.getEntities()) {
      if (entity->onUse(*this)) {
        floor.remove(*entity);
        this->onTurnEnd();
        return;
      }
    }
    throw std::invalid_argument("No potion in the given direction.");
  }

  Attack attack = std::get<Attack>(nextMove);
  const Cell& c = floor.getCell(this->position() + attack.position);

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
  this->onTurnEnd();
}

void Character::drain(Character& defender, unsigned int hp) {
  if (const double mult = defender.getDrainMulti(*this); mult > 0.0) {
    this->heal(hp * mult);
  } else {
    this->damage(hp * static_cast<int>(-mult));
  }
}

void Character::onDeath(Character* killer) {
  if (!killer) return;
  killer->onKill(*this);
}

void Character::onKill(const Character& killed) {
  if (const unsigned int amt = killed.getGoldDrop().instantAmount) {
    this->addGold(amt);
  }
}
