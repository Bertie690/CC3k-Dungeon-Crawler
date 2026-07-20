module game;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../entities/enemy.cc"
#include "../entities/entity.cc"
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
import <vector>;
import enemy;
import entity;
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

Game::Game(unique_ptr<Renderer> renderer, const string& floorFile, const int seed)
    : rng{seed}, scoreboard{}, renderer{move(renderer)}, floorGenerator{}, floor{}, player{} {
  if (floorFile.empty()) {
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  } else {
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile);
  }
  attach(this->renderer.get());
}

void Game::newGame() {
  floorTransitionRequested = false;
  // (0,0) position outside of Chambers until we place Player on the first Floor
  player = make_shared<Player>(Position{0, 0}, Stats{125, 25, 25}, RaceType::Shade);
  loadNextFloor();
  // TODO: start turns
}

void Game::loadNextFloor() {
  floor = make_unique<Floor>(floorGenerator->generateFloor());
  floor->Subject<EntityMoveEvent>::attach(renderer.get());
  floor->Subject<FloorTransitionEvent>::attach(this);

  // Player is added outside of FloorGenerator to share Ownership with the Game
  player->position = floor->playerSpawn;
  floor->getCell(player->position).add(player);

  notify(NewFloorEvent{floor.get()});
  renderer->draw();
}

void Game::onNotify(const FloorTransitionEvent&) { floorTransitionRequested = true; }

void Game::runEnemyTurn() {
  // Player reached staircase, enemy turn is skipped and next floor is loaded
  if (floorTransitionRequested) {
    floorTransitionRequested = false;
    loadNextFloor();
    return;
  }

  vector<shared_ptr<Enemy>> enemies;

  // Add all Enemies in Chambers to the enemies vector
  for (const Room* room : floor->getRooms()) {
    if (room->type() != RoomType::Chamber) continue;

    for (const Cell* cell : room->getCells()) {
      for (const shared_ptr<Entity>& entity : cell->getEntities()) {
        shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(entity);
        if (enemy) enemies.push_back(enemy);
      }
    }
  }

  // TODO: could possibly be optimized
  sort(enemies.begin(), enemies.end(), [](const shared_ptr<Enemy>& a, const shared_ptr<Enemy>& b) {
    return a->position < b->position;
  });

  // Run each Enemy's turn
  for (const shared_ptr<Enemy>& enemy : enemies) {
    enemy->act(*floor);
  }
  renderer->draw();
}
