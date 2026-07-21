export module renderer;

#pragma once

#ifdef __INTELLISENSE__
#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#else
import gameevents;
import floorevents;
import observer;
#endif  // __INTELLISENSE__

export class Renderer : public Observer<NewFloorEvent, EntityDeathEvent, EntityMoveEvent> {
 public:
  virtual ~Renderer() = default;

  // Draw the current state of the game to the screen.
  virtual void draw() = 0;
};
