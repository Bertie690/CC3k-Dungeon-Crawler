export module game;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <string>

#include "../display/renderer.cc"
#include "../entities/player.cc"
#include "../enums/action.cc"
#include "../enums/race-type.cc"
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
import racetype;
import floorgenerator;
import floor;
import rng;
import scoreboard;
#endif  // __INTELLISENSE__

using namespace std;

// Game owns and manages the main game objects
export class Game : public Observer<FloorTransitionEvent, GameQuitEvent, PlayerActionEvent>,
                    public Subject<NewFloorEvent, PlayerActionEvent> {
  RNG rng;
  Scoreboard scoreboard;
  unique_ptr<Renderer> renderer;
  unique_ptr<FloorGenerator> floorGenerator;
  unique_ptr<Floor> floor;
  shared_ptr<Player> player = {};
  unsigned int floorNumber = 0;
  std::string lastAction = "Starting a new game";
  // Flag used to know to generate the next Floor instead of running enemy turns.
  bool floorTransitionRequested = false;

  // Generate a Floor and move the Player to its determined spawn position.
  void loadNextFloor();
  PlayerDisplayInfo playerDisplayInfo() const;

  virtual void onNotify(const FloorTransitionEvent& event) override;
  virtual void onNotify(const GameQuitEvent& event) override;
  virtual void onNotify(const PlayerActionEvent& event) override;

 public:
  // Create a new Game with the given Renderer, floor file, and RNG seed.
  Game(unique_ptr<Renderer> renderer, const string& floorFile, const int seed);
  // Create a new Game with the given Renderer, floor file, and the default RNG seed.
  Game(unique_ptr<Renderer> renderer, const string& floorFile = "");

  // Initialize a new game with the Player race.
  void newGame(RaceType race);
  // Run a turn for the Player.
  void runPlayerTurn(const PlayerAction& action);
  // Run one turn for every Enemy currently on the Floor.
  void runEnemyTurn();

  // TODO: add InputHandler
};
