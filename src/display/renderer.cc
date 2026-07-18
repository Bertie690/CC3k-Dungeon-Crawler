export module renderer;

#pragma once

#ifdef __INTELLISENSE__
#include "../events/game-events.cc"
#include "../events/observer.cc"
#else
import gameevents;
import observer;
#endif  // __INTELLISENSE__

// TODO: Should the base Renderer be the Observer for all events?
// Would we rather NewFloorEvent just be with the others in TUI?
export class Renderer : public Observer<NewFloorEvent> {
 public:
  virtual ~Renderer() = default;
  virtual void draw() = 0;
  virtual void onNotify(const NewFloorEvent& event) override = 0;
};
