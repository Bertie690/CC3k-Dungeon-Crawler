module game;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "../entities/character.cc"
#include "../entities/enemy.cc"
#include "../entities/entity.cc"
#include "../entities/merchant.cc"
#include "../entities/player.cc"
#include "../entities/potion.cc"
#include "../entities/stats.cc"
#include "../enums/direction.cc"
#include "../enums/gold-size.cc"
#include "../enums/potion-type.cc"
#include "../enums/race-type.cc"
#include "../enums/room-type.cc"
#include "../events/floor-events.cc"
#include "../floor/cell.cc"
#include "../floor/position.cc"
#include "../floor/preset-floor-generator.cc"
#include "../floor/random-floor-generator.cc"
#include "../floor/room.cc"
#include "feature-flags.cc"
#include "game.cc"
#else
import <algorithm>;
import <cstdlib>;
import <memory>;
import <string>;
import <utility>;
import <variant>;
import <vector>;
import enemy;
import entity;
import merchant;
import character;
import player;
import potion;
import potiontype;
import direction;
import stats;
import racetype;
import roomtype;
import cell;
import position;
import floorevents;
import goldsize;
import presetfloorgenerator;
import randomfloorgenerator;
import room;
import featureflags;
#endif  // __INTELLISENSE__

using namespace std;

namespace {
  constexpr string characterName(const RaceType type) noexcept {
    try {
      if (isPlayer(type)) return "PC";
    } catch (...) {
      return "unknown entity";
    }

    switch (type) {
      case RaceType::Human:
        return "H";
      case RaceType::Dwarf:
        return "W";
      case RaceType::Elf:
        return "E";
      case RaceType::Orc:
        return "O";
      case RaceType::Merchant:
        return "M";
      case RaceType::Dragon:
        return "D";
      case RaceType::Halfling:
        return "L";
      default:
        return "Unknown enemy";
    }
  }

  constexpr string characterName(const Entity& entity) noexcept {
    if (const Character* character = dynamic_cast<const Character*>(&entity)) {
      return characterName(character->raceType());
    }
    return "unknown entity";
  }

  string directionName(const Position& from, const Position& to) {
    if (to.y < from.y) {
      if (to.x < from.x) return "north-west";
      if (to.x > from.x) return "north-east";
      return "north";
    }
    if (to.y > from.y) {
      if (to.x < from.x) return "south-west";
      if (to.x > from.x) return "south-east";
      return "south";
    }
    return to.x < from.x ? "west" : "east";
  }

  constexpr string potionName(const PotionType type) noexcept {
    switch (type) {
      case PotionType::RestoreHealth:
        return "RH";
      case PotionType::BoostAttack:
        return "BA";
      case PotionType::BoostDefense:
        return "BD";
      case PotionType::PoisonHealth:
        return "PH";
      case PotionType::WoundAttack:
        return "WA";
      case PotionType::WoundDefense:
        return "WD";
    }
    return "unknown potion";
  }
}  // namespace

Game::Game(unique_ptr<Renderer> renderer, const string& floorFile, const int seed)
    : rng{seed},
      scoreboard{},
      playerFactory{rng},
      goldFactory{rng},
      floorFile{floorFile},
      renderer{move(renderer)},
      floorGenerator{},
      floor{} {
  if (floorFile.empty()) {
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  } else {
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile,
                                                       FeatureFlags::enableImprovedHoardPlacement);
  }

  attach(this->renderer.get());
  this->renderer->drawStartScreen();
}

Game::Game(unique_ptr<Renderer> renderer, const string& floorFile)
    : rng{},
      scoreboard{},
      playerFactory{rng},
      goldFactory{rng},
      floorFile{floorFile},
      renderer{move(renderer)},
      floorGenerator{},
      floor{} {
  if (floorFile.empty()) {
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  } else {
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile,
                                                       FeatureFlags::enableImprovedHoardPlacement);
  }

  attach(this->renderer.get());
  this->renderer->drawStartScreen();
}

void Game::newGame(RaceType race) {
  pendingGameOver = false;
  scoreboard.score = 0;
  floorTransitionRequested = false;
  floorNumber = 0;
  discoveredPotions.clear();
  actionLog.set("Started a new game");
  Merchant::resetHostility();
  freezeEnemies(false);
  if (floorFile.empty()) {
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  } else {
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile);
  }
  if (player) {
    if (Observer<PlayerActionEvent>* observer = player->inputObserver()) {
      detach(observer);
    }
    if (Observer<NewFloorEvent>* observer = player->newFloorObserver()) {
      detach(observer);
    }
  }
  // (0,0) position outside of Chambers until we place Player on the first Floor
  player = playerFactory.create(Position{0, 0}, race);
  goldAtTurnStart = player->getGold();
  if (Observer<PlayerActionEvent>* observer = player->inputObserver()) {
    attach(observer);
  }
  if (Observer<NewFloorEvent>* observer = player->newFloorObserver()) {
    attach(observer);
  }
  loadNextFloor();
  // TODO: start turns
}

void Game::loadNextFloor() {
  ++floorNumber;
  floor = make_unique<Floor>(floorGenerator->generateFloor());
  floor->attach(renderer.get());
  floor->attach(this);

  // Player is added outside of FloorGenerator to share ownership with the Game
  player->position() = floor->playerSpawn;
  floor->getCell(player->position()).add(player);

  notify(NewFloorEvent{floor.get()});
  renderer->draw(playerDisplayInfo());
}

PlayerDisplayInfo Game::playerDisplayInfo() const {
  return PlayerDisplayInfo{player->raceType(),  player->getGold(),  floorNumber,
                           player->currentHp(), player->getStats(), actionLog.text()};
}

void Game::endGame(bool victory) {
  scoreboard.score = victory ? player->getGold() * player->getScoreMulti() : 0;
  actionLog.set(victory ? "Reached the end of the game" : "Player died");
  pendingGameOver = true;
  floorTransitionRequested = false;
  renderer->draw(playerDisplayInfo());
  renderer->drawGameOutcome(victory, scoreboard.score);
  notify(GameOverEvent{});
}

void Game::onNotify(const FloorTransitionEvent&) { floorTransitionRequested = true; }

void Game::onNotify(const EntityMoveEvent& event) {
  if (!dynamic_cast<const Character*>(&event.entity)) return;

  const bool isPlayerMove = &event.entity == player.get();
  const Position& playerPosition = player->position();
  const bool movedNextToPlayer =
      !isPlayerMove && event.to.x >= playerPosition.x - 1 && event.to.x <= playerPosition.x + 1 &&
      event.to.y >= playerPosition.y - 1 && event.to.y <= playerPosition.y + 1;

  if (isPlayerMove || movedNextToPlayer) {
    actionLog.append(characterName(event.entity) + " moves " + directionName(event.from, event.to));
  }
  if (isPlayerMove && player->getGold() > goldAtTurnStart) {
    actionLog.append("PC picks up " + to_string(player->getGold() - goldAtTurnStart) + " gold");
    goldAtTurnStart = player->getGold();
  }
  if (isPlayerMove) {
    for (const Direction direction : allDirections) {
      const Position potionPosition = playerPosition + direction;
      if (!floor->hasCell(potionPosition)) continue;
      for (const shared_ptr<Entity>& entity : floor->getCell(potionPosition).getEntities()) {
        const Potion* potion = dynamic_cast<const Potion*>(entity.get());
        if (!potion) continue;
        if (discoveredPotions.find(potion->potionType) == discoveredPotions.end()) {
          actionLog.append("PC sees an unknown potion");
        } else {
          actionLog.append("PC sees " + potionName(potion->potionType));
        }
      }
    }
  }
}

void Game::onNotify(const CharacterAttackEvent& event) {
  const string attacker = characterName(event.attacker);
  const string defender = characterName(event.defender);

  if (event.result == CharacterAttackEvent::Result::Miss) {
    actionLog.append(attacker + " misses " + defender);
    return;
  }

  string message = attacker + " deals " + to_string(event.damage) + " damage to " + defender;

  if (&event.attacker == player.get() && !event.defeated) {
    const Character* defendingCharacter = dynamic_cast<const Character*>(&event.defender);
    if (defendingCharacter) {
      message += " (" + to_string(defendingCharacter->currentHp()) + " HP)";
    }
  }
  if (event.defeated) message += " and defeats " + defender;
  actionLog.append(message);

  if (&event.attacker == player.get() && player->getGold() > goldAtTurnStart) {
    actionLog.append("PC gains " + to_string(player->getGold() - goldAtTurnStart) + " gold");
    goldAtTurnStart = player->getGold();
  }
}

void Game::onNotify(const RaceSelectEvent& event) { newGame(event.raceType); }

void Game::onNotify(const RestartEvent&) { renderer->drawStartScreen(); }

void Game::onNotify(const FreezeEnemiesEvent&) {
  if (pendingGameOver) return;
  freezeEnemies();
  actionLog.set("PC toggles enemy movement");
  renderer->draw(playerDisplayInfo());
}
void Game::onNotify(const PlayerActionEvent& event) {
  if (pendingGameOver) return;
  runTurnCycle(event.action);
}

void Game::runTurnCycle(const Action& action) {
  actionLog.clear();
  goldAtTurnStart = player->getGold();
  if (holds_alternative<Pass>(action)) actionLog.append("PC waits");
  if (const UsePotion* use = get_if<UsePotion>(&action)) {
    const Position potionPosition = player->position() + use->dir;
    if (floor->hasCell(potionPosition)) {
      for (const auto& entity : floor->getCell(potionPosition).getEntities()) {
        if (const Potion* potion = dynamic_cast<const Potion*>(entity.get())) {
          discoveredPotions.insert(potion->potionType);
          actionLog.append("PC uses " + potionName(potion->potionType));
          break;
        }
      }
    }
    if (actionLog.empty()) actionLog.append("PC uses a potion");
  }
  notify(PlayerActionEvent{action});
  player->act(*floor);

  // Player reached staircase, enemy turn is skipped and next floor is loaded
  if (floorTransitionRequested) {
    if (floorNumber == MAX_NUM_FLOORS) {
      endGame(true);
      return;
    }
    floorTransitionRequested = false;
    actionLog.append("PC descends to the next floor");
    loadNextFloor();
    return;
  }
  runEnemyTurn();

  if (pendingGameOver) {
    endGame(false);
    return;
  }
}

void Game::runEnemyTurn() {
  vector<shared_ptr<Character>> enemies;

  // Add all Enemies in Chambers to the enemies vector
  for (const Room* room : floor->getRooms()) {
    if (room->type() != RoomType::Chamber) continue;

    for (const Cell* cell : room->getCells()) {
      for (const shared_ptr<Entity>& entity : cell->getEntities()) {
        shared_ptr<Character> character = dynamic_pointer_cast<Character>(entity);
        if (character && !isPlayer(character->raceType())) enemies.push_back(character);
      }
    }
  }

  // TODO: could possibly be optimized
  sort(enemies.begin(), enemies.end(),
       [](const shared_ptr<Character>& a, const shared_ptr<Character>& b) {
         return a->position() < b->position();
       });

  // Run each Enemy's turn
  for (const shared_ptr<Character>& enemy : enemies) {
    if (player->isDead() || pendingGameOver) break;
    enemy->act(*floor);
  }
  renderer->draw(playerDisplayInfo());
}

void Game::onNotify(const GameQuitEvent&) {}

void Game::onNotify(const CharacterDeathEvent& event) {
  notify(event);

  const Position& playerPosition = player->position();
  if (&event.entity == player.get()) {
    // "Game Over, Man! Game Over!"
    pendingGameOver = true;
    return;
  }

  const bool playerKilledEnemy = event.killerPosition == playerPosition;
  if (!playerKilledEnemy) {
    return;
  }

  // TODO: move action messages into wherever the action bar is moved

  string message = "PC killed " + characterName(event.entity);

  // create the enemy's physical GoldPile(s)
  if (const unsigned int goldDropped = event.goldDrop.pilesDropped) {
    message += " which drops " + to_string(goldDropped) + " piles of gold";
    for (unsigned int i = 0; i < goldDropped; ++i) {
      floor->getCell(event.position)
          .add(goldFactory.create(event.position, event.goldDrop.pileSize));
    }
  }

  actionLog.append(message);
}
