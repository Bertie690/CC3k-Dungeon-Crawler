module game;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../entities/enemy.cc"
#include "../entities/entity.cc"
#include "../enums/room-type.cc"
#include "../floor/cell.cc"
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
import roomtype;
import cell;
import presetfloorgenerator;
import randomfloorgenerator;
import room;
#endif  // __INTELLISENSE__

using namespace std;

Game::Game(unique_ptr<Renderer> renderer, const string& floorFile, const int seed)
    : rng{seed}, scoreboard{}, renderer{move(renderer)}, floorGenerator{}, floor{} {
  if (floorFile.empty())
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  else
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile);

  attach(this->renderer.get());
}

void Game::newGame() {
  floor = make_unique<Floor>(floorGenerator->generateFloor());
  floor->Subject<EntityMoveEvent>::attach(renderer.get());
  floor->Subject<FloorTransitionEvent>::attach(this);

  // TODO: create Player on first Floor (PlayerFactory)
  // TODO: Set Player->position = floor->playerSpawn and share with the Cell
  notify(NewFloorEvent{floor.get()});
  renderer->draw();
  // TODO: start turns
}

void Game::onNotify(const FloorTransitionEvent&) {
  floorTransitionRequested = true;
  // Flag used to know to generate the next Floor instead of running enemy turns
}

void Game::runEnemyTurn() {
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
