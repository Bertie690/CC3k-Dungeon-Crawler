export module renderer;

#pragma once

#ifdef __INTELLISENSE__
#include <string>

#include "../entities/stats.cc"
#include "../enums/race-type.cc"
#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#else
import <string>;
import stats;
import racetype;
import gameevents;
import floorevents;
import observer;
#endif  // __INTELLISENSE__

export struct PlayerDisplayInfo {
  RaceType race;
  int gold;
  unsigned int floorNumber;
  unsigned int hp;
  Stats stats;
  std::string action;
};

export class Renderer : public Observer<NewFloorEvent, EntityDeathEvent, EntityMoveEvent> {
 public:
  virtual ~Renderer() = default;

  // Display the title screen before a race is selected.
  virtual void drawStartScreen() = 0;
  // Draw the current state of the game to the screen.
  virtual void draw(const PlayerDisplayInfo& info) = 0;
  // Display the outcome of a game.
  virtual void drawGameOutcome(bool victory, double score) = 0;
};
