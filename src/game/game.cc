export module game;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <set>
#include <string>

#include "../display/renderer.cc"
#include "../entities/character.cc"
#include "../entities/player.cc"
#include "../entities/potion.cc"
#include "../enums/action.cc"
#include "../enums/potion-type.cc"
#include "../enums/race-type.cc"
#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../factories/gold-factory.cc"
#include "../factories/player-factory.cc"
#include "../floor/floor-generator.cc"
#include "../floor/floor.cc"
#include "../utils/rng.cc"
#include "action-log.cc"
#include "scoreboard.cc"
#else
import <memory>;
import <set>;
import <string>;
import character;
import renderer;
import player;
import potion;
import potiontype;
import floorevents;
import gameevents;
import observer;
import goldfactory;
import playerfactory;
import action;
import racetype;
import floorgenerator;
import floor;
import floorevents;
import rng;
import actionlog;
import scoreboard;
#endif  // __INTELLISENSE__

using namespace std;

// Game owns and manages the main game objects
export class Game
    : public Observer<FloorTransitionEvent, GameQuitEvent, PlayerActionEvent, EntityMoveEvent,
                      CharacterAttackEvent, CharacterDeathEvent, FreezeEnemiesEvent,
                      RaceSelectEvent, RestartEvent>,
      public Subject<NewFloorEvent, PlayerActionEvent, CharacterDeathEvent, GameOverEvent> {
  RNG rng;
  Scoreboard scoreboard;
  PlayerFactory playerFactory;
  GoldFactory goldFactory;
  const string floorFile;
  unique_ptr<Renderer> renderer;
  unique_ptr<FloorGenerator> floorGenerator;
  unique_ptr<Floor> floor;
  shared_ptr<Character> player = {};
  unsigned int floorNumber = 0;
  std::set<PotionType> discoveredPotions;
  ActionLog actionLog{"Starting a new game"};
  // Whether a game over is pending. Will be checked at the end of every single turn.
  bool pendingGameOver = false;
  int goldAtTurnStart = 0;
  // Flag used to know to generate the next Floor instead of running enemy turns.
  bool floorTransitionRequested = false;

  // Generate a Floor and move the Player to its determined spawn position.
  void loadNextFloor();
  // End the game and calculate a score if game was won.
  void endGame(bool victory);
  PlayerDisplayInfo playerDisplayInfo() const;

  virtual void onNotify(const FloorTransitionEvent& event) override;
  virtual void onNotify(const GameQuitEvent& event) override;
  virtual void onNotify(const PlayerActionEvent& event) override;
  virtual void onNotify(const EntityMoveEvent& event) override;
  virtual void onNotify(const CharacterAttackEvent& event) override;
  virtual void onNotify(const CharacterDeathEvent& event) override;
  virtual void onNotify(const FreezeEnemiesEvent& event) override;
  virtual void onNotify(const RaceSelectEvent& event) override;
  virtual void onNotify(const RestartEvent& event) override;

  // Run a full turn cycle with the specified player action.
  void runTurnCycle(const Action& playerAction);
  // Run one turn for every Enemy currently on the Floor.
  void runEnemyTurn();
  // Initialize a new game with the Player race.
  void newGame(RaceType race);

 public:
  // Create a new Game with the given Renderer, floor file, and RNG seed.
  Game(unique_ptr<Renderer> renderer, const string& floorFile, const int seed);
  // Create a new Game with the given Renderer, floor file, and the default RNG seed.
  Game(unique_ptr<Renderer> renderer, const string& floorFile = "");
};
