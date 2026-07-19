export module game;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <string>

#include "../display/renderer.cc"
#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/floor-generator.cc"
#include "../floor/floor.cc"
#include "../utils/rng.cc"
#include "scoreboard.cc"
#else
import <memory>;
import <string>;
import renderer;
import floorevents;
import gameevents;
import observer;
import floorgenerator;
import floor;
import rng;
import scoreboard;
#endif  // __INTELLISENSE__

using namespace std;

// Game owns and manages the main game objects
export class Game : public Observer<FloorTransitionEvent>, public Subject<NewFloorEvent> {
  RNG rng;
  Scoreboard scoreboard;
  unique_ptr<Renderer> renderer;
  unique_ptr<FloorGenerator> floorGenerator;
  unique_ptr<Floor> floor;
  // Flag used to know to generate the next Floor instead of running enemy turns.
  bool floorTransitionRequested = false;

  // TODO: shared_ptr<Player>

  // Create the next Floor when a floor transition is requested.
  virtual void onNotify(const FloorTransitionEvent& event) override;

 public:
  Game(unique_ptr<Renderer> renderer, const string& floorFile = "", const int seed = 0);
  // Initialize a new game
  void newGame();
  // Run one turn for every Enemy currently on the Floor.
  void runEnemyTurn();

  // TODO: add InputHandler
};
