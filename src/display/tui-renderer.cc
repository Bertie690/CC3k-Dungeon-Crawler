export module tuirenderer;

#pragma once

#ifdef __INTELLISENSE__
#include <string>
#include <vector>

#include "../entities/entity.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "renderer.cc"
#else
import renderer;
import observer;
import gameevents;
import entity;
import floor;
import cell;
import <vector>;
import <string>;
#endif

export class TUIRenderer : public Renderer,
                           public Observer<EntityDeathEvent, EntityMoveEvent, NewFloorEvent> {
  // TODO: unused
  const Floor* currentFloor;
  std::vector<std::vector<std::string>> displayGrid;

  char determineSymbol(const Cell& cell) const;
  char getEntitySymbol(const Entity& entity) const noexcept;
  void rebuildGrid();

 public:
  TUIRenderer();
  void draw() override;
  void onNotify(const EntityDeathEvent& event) override;
  void onNotify(const EntityMoveEvent& event) override;
  void onNotify(const NewFloorEvent& event) override;
};
