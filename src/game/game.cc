export module game;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <string>

#include "../display/renderer.cc"
#include "../entities/player.cc"
#include "../enums/action.cc"
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
import player;
import floorevents;
import gameevents;
import observer;
import action;
import floorgenerator;
import floor;
import rng;
import scoreboard;
#endif  // __INTELLISENSE__

using namespace std;

// Game owns and manages the main game objects
export class Game : public Observer<FloorTransitionEvent>,
                    public Subject<NewFloorEvent>,
                    public Subject<PlayerActionEvent> {
  RNG rng;
  Scoreboard scoreboard;
  unique_ptr<Renderer> renderer;
  unique_ptr<FloorGenerator> floorGenerator;
  unique_ptr<Floor> floor;
  shared_ptr<Player> player;
  // Flag used to know to generate the next Floor instead of running enemy turns.
  bool floorTransitionRequested = false;

  // Generate a Floor and move the Player to its determined spawn position.
  void loadNextFloor();

  // Create the next Floor when a floor transition is requested.
  virtual void onNotify(const FloorTransitionEvent& event) override;

 public:
  // TODO: uses the 0 seed everytime, may want to make overload for with and without provided seed
  // so it can default to the RNG default seed
  Game(unique_ptr<Renderer> renderer, const string& floorFile = "", const int seed = 0);
  // Initialize a new game
  void newGame();
  // Run turn for the Player.
  void runPlayerTurn(const PlayerAction& action);
  // Run one turn for every Enemy currently on the Floor.
  void runEnemyTurn();

  // TODO: add InputHandler
};
