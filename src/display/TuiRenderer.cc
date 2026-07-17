export module tui_renderer;

#pragma once

#ifdef __INTELLISENSE__
#include <string>
#include <vector>

#include "floor.cc"
#include "game_events.cc"
#include "observer.cc"
#include "renderer.cc"
#else
import renderer;
import observer;
import game_events;
import floor;
import <vector>;
import <string>;
#endif

using namespace std;

export class TUIRenderer : public Renderer,
                           public Observer<EntityDeathEvent, MoveEvent, NewFloorEvent> {
  const Floor* currentFloor;
  vector<vector<string>> displayGrid;

  string determineSymbol(const Cell& cell) const;
  string getEntitySymbol(const Entity& entity) const noexcept;
  void rebuildGrid();

 public:
  TUIRenderer();
  void draw() override;
  void onNotify(const EntityDeathEvent& event) override;
  void onNotify(const MoveEvent& event) override;
  void onNotify(const NewFloorEvent& event) override;
};
