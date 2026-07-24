module game;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "../entities/enemy.cc"
#include "../entities/entity.cc"
#include "../entities/character.cc"
#include "../entities/player.cc"
#include "../entities/stats.cc"
#include "../enums/race-type.cc"
#include "../enums/room-type.cc"
#include "../floor/cell.cc"
#include "../floor/position.cc"
#include "../floor/preset-floor-generator.cc"
#include "../floor/random-floor-generator.cc"
#include "../floor/room.cc"
#include "game.cc"
#else
import <algorithm>;
import <memory>;
import <string>;
import <utility>;
import <variant>;
import <vector>;
import enemy;
import entity;
import character;
import player;
import stats;
import racetype;
import roomtype;
import cell;
import position;
import presetfloorgenerator;
import randomfloorgenerator;
import room;
#endif  // __INTELLISENSE__

using namespace std;

namespace {
  string characterName(const Entity& entity) {
    const Character* character = dynamic_cast<const Character*>(&entity);
    if (!character) return "unknown entity";
    if (isPlayer(character->raceType())) return "PC";

    switch (character->raceType()) {
      case RaceType::Human: return "H";
      case RaceType::Dwarf: return "W";
      case RaceType::Elf: return "E";
      case RaceType::Orc: return "O";
      case RaceType::Merchant: return "M";
      case RaceType::Dragon: return "D";
      case RaceType::Halfling: return "L";
      default: return "Enemy";
    }
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
}

Game::Game(unique_ptr<Renderer> renderer, const string& floorFile, const int seed)
    : rng{seed}, scoreboard{}, playerFactory{rng}, renderer{move(renderer)}, floorGenerator{}, floor{} {
  if (floorFile.empty()) {
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  } else {
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile);
  }

  attach(this->renderer.get());
}

Game::Game(unique_ptr<Renderer> renderer, const string& floorFile)
    : rng{}, scoreboard{}, playerFactory{rng}, renderer{move(renderer)}, floorGenerator{}, floor{} {
  if (floorFile.empty()) {
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  } else {
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile);
  }

  attach(this->renderer.get());
}

void Game::newGame(RaceType race) {
  gameOver = false;
  scoreboard.score = 0;
  floorTransitionRequested = false;
  floorNumber = 0;
  lastAction = "Started a new game";
  // TODO reset other state like merchant hostility
  if (player) {
    detach(player->inputObserver());
  }
  // (0,0) position outside of Chambers until we place Player on the first Floor
  player = playerFactory.create(Position{0, 0}, race);
  goldAtTurnStart = player->getGold();
  attach(player->inputObserver());
  loadNextFloor();
  // TODO: start turns
}

void Game::loadNextFloor() {
  ++floorNumber;
  floor = make_unique<Floor>(floorGenerator->generateFloor());
  floor->SubjectFor<EntityMoveEvent>::attach(renderer.get());
  floor->SubjectFor<EntityDeathEvent>::attach(renderer.get());
  floor->SubjectFor<FloorTransitionEvent>::attach(this);
  floor->SubjectFor<EntityMoveEvent>::attach(this);
  floor->SubjectFor<CharacterActionEvent>::attach(this);

  // Player is added outside of FloorGenerator to share ownership with the Game
  player->position() = floor->playerSpawn;
  floor->getCell(player->position()).add(player);

  notify(NewFloorEvent{floor.get()});
  renderer->draw(playerDisplayInfo());
}

PlayerDisplayInfo Game::playerDisplayInfo() const {
  return PlayerDisplayInfo{player->raceType(),  player->getGold(), floorNumber,
                           player->currentHp(), player->stats(), lastAction};
}

void Game::endGame(bool victory) {
  gameOver = true;
  scoreboard.score = victory ? player->getGold() * player->getScoreMulti() : 0;
  lastAction = victory ? "Reached the end of the game" : "Player died";
  floorTransitionRequested = false;
  renderer->draw(playerDisplayInfo());
  renderer->drawGameOutcome(victory, scoreboard.score);
}

void Game::onNotify(const FloorTransitionEvent&) { floorTransitionRequested = true; }

void Game::appendAction(const string& message) {
  if (!lastAction.empty()) lastAction += "; ";
  lastAction += message;
}

void Game::onNotify(const EntityMoveEvent& event) {
  if (!dynamic_cast<const Character*>(&event.entity)) return;

  const bool isPlayerMove = &event.entity == player.get();
  const Position& playerPosition = player->position();
  const bool movedNextToPlayer = !isPlayerMove &&
      event.to.x >= playerPosition.x - 1 && event.to.x <= playerPosition.x + 1 &&
      event.to.y >= playerPosition.y - 1 && event.to.y <= playerPosition.y + 1;

  if (isPlayerMove || movedNextToPlayer) {
    appendAction(characterName(event.entity) + " moves " + directionName(event.from, event.to));
  }
  if (isPlayerMove && player->getGold() > goldAtTurnStart) {
    appendAction("PC picks up " + to_string(player->getGold() - goldAtTurnStart) + " gold");
    goldAtTurnStart = player->getGold();
  }
}

void Game::onNotify(const CharacterActionEvent& event) {
  const string attacker = characterName(event.attacker);
  const string defender = characterName(event.defender);

  if (event.result == CharacterActionEvent::Result::Miss) {
    appendAction(attacker + " misses " + defender);
    return;
  }

  string message = attacker + " deals " + to_string(event.damage) + " damage to " + defender;

  if (event.defeated) message += " and defeats " + defender;
  appendAction(message);

  if (&event.attacker == player.get() && player->getGold() > goldAtTurnStart) {
    appendAction("PC gains " + to_string(player->getGold() - goldAtTurnStart) + " gold");
    goldAtTurnStart = player->getGold();
  }
}

void Game::onNotify(const PlayerActionEvent& event) {
  if (const RaceSelect* raceSelect = get_if<RaceSelect>(&event.action)) {
    newGame(raceSelect->race);
    return;
  }
  if (gameOver) {
    return;
  }
  if (const FreezeEnemies* freeze = get_if<FreezeEnemies>(&event.action)) {
    freezeEnemies();
    lastAction = "PC toggles enemy movement";
    renderer->draw(playerDisplayInfo());
    return;
  }
  runPlayerTurn(event.action);
}

void Game::runPlayerTurn(const PlayerAction& action) {
  lastAction.clear();
  goldAtTurnStart = player->getGold();
  if (holds_alternative<Pass>(action)) appendAction("PC waits");
  if (holds_alternative<UsePotion>(action)) appendAction("PC uses a potion");
  notify(PlayerActionEvent{action});
  player->act(*floor);
  if (player->dead()) {
    endGame(false);
    return;
  }
  player->endTurn();

  // Player reached staircase, enemy turn is skipped and next floor is loaded
  if (floorTransitionRequested) {
    if (floorNumber == 5) {
      endGame(true);
      return;
    }
    floorTransitionRequested = false;
    appendAction("PC descends to the next floor");
    loadNextFloor();
    return;
  }
  runEnemyTurn();
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
  sort(enemies.begin(), enemies.end(), [](const shared_ptr<Character>& a, const shared_ptr<Character>& b) {
    return a->position() < b->position();
  });

  // Run each Enemy's turn
  for (const shared_ptr<Character>& enemy : enemies) {
    enemy->act(*floor);
    if (player->dead()) {
      endGame(false);
      return;
    }
  }
  renderer->draw(playerDisplayInfo());
}

void Game::onNotify(const GameQuitEvent& event) {
  // TODO: Display scoreboard and end the game
  // if (event.showScoreboard) {
}
