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
  string actionDescription(const PlayerAction& action) {
    if (holds_alternative<Move>(action)) return "Moved";
    if (holds_alternative<Attack>(action)) return "Attacked";
    if (holds_alternative<UsePotion>(action)) return "Used a potion";
    if (holds_alternative<FreezeEnemies>(action)) return "Toggled enemy movement";
    if (holds_alternative<Pass>(action)) return "Waited";
    return "Selected a race";
  }
}  // namespace

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
  attach(player->inputObserver());
  loadNextFloor();
  // TODO: start turns
}

void Game::loadNextFloor() {
  ++floorNumber;
  floor = make_unique<Floor>(floorGenerator->generateFloor());
  floor->SubjectFor<EntityMoveEvent>::attach(renderer.get());
  floor->SubjectFor<EntityDeathEvent>::attach(renderer.get());
  floor->attach(this);

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
    lastAction = actionDescription(event.action);
    renderer->draw(playerDisplayInfo());
    return;
  }
  runPlayerTurn(event.action);
}

void Game::runPlayerTurn(const PlayerAction& action) {
  lastAction = actionDescription(action);
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
