module game;

#ifdef __INTELLISENSE__
#include <memory>
#include <string>
#include <utility>

#include "../floor/preset-floor-generator.cc"
#include "../floor/random-floor-generator.cc"
#include "game.cc"
#else
import <memory>;
import <string>;
import <utility>;
import presetfloorgenerator;
import randomfloorgenerator;
#endif  // __INTELLISENSE__

using namespace std;

Game::Game(int seed, unique_ptr<Renderer> renderer, const string& floorFile)
    : rng{seed}, scoreboard{}, renderer{move(renderer)}, floorGenerator{}, floor{} {
  if (floorFile.empty())
    floorGenerator = make_unique<RandomFloorGenerator>(rng);
  else
    floorGenerator = make_unique<PresetFloorGenerator>(rng, floorFile);

  attach(this->renderer.get());
}

void Game::newGame() {
  floor = make_unique<Floor>(floorGenerator->generateFloor());

  // TODO: create Player on first Floor (PlayerFactory)
  // TODO: Set Player->position = floor->playerSpawn and share with the Cell
  notify(NewFloorEvent{floor.get()});
  // TODO: start turns
}

void Game::onNotify(const FloorTransitionEvent&) {
  floor = make_unique<Floor>(floorGenerator->generateFloor());

  // TODO:Set Player->position = floor->playerSpawn and share with the Cell
  notify(NewFloorEvent{floor.get()});
  // TODO: start turns on new Floor
}
