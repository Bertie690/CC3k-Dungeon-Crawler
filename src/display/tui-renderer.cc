export module tuirenderer;

#pragma once

#ifdef __INTELLISENSE__
#include <string>
#include <vector>

#include "../entities/entity.cc"
#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "../events/observer.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "renderer.cc"
#else
import renderer;
import observer;
import floorevents;
import gameevents;
import entity;
import floor;
import cell;
import <vector>;
import <string>;
#endif

export class TUIRenderer : public Renderer {
  const Floor* currentFloor;
  // The underlying grid of strings to be displayed, stored in row-major order.
  std::vector<std::vector<std::string>> displayGrid;

  // Return the text to be displayed for the given Cell.
  std::string getCellText(const Cell& cell) const;
  // Return the text to be displayed for the given Entity.
  std::string getEntitySymbol(const Entity& entity) const noexcept;
  void rebuildGrid();

  virtual void onNotify(const EntityDeathEvent& event) override;
  virtual void onNotify(const EntityMoveEvent& event) override;
  virtual void onNotify(const NewFloorEvent& event) override;

 public:
  TUIRenderer();
  virtual void draw() override;
};
