export module x11renderer;

#pragma once

#ifdef __INTELLISENSE__
#include <string>

#include "../entities/entity.cc"
#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "renderer.cc"
#else
import <string>;
import entity;
import floorevents;
import gameevents;
import cell;
import floor;
import renderer;
#endif

export class X11Renderer final : public Renderer {
  void* display = nullptr;
  unsigned long window = 0;
  void* graphicsContext = nullptr;
  const Floor* currentFloor = nullptr;
  int screen = 0;

  unsigned long black = 0;
  unsigned long white = 0;
  unsigned long wall = 0;
  unsigned long floor = 0;
  unsigned long passage = 0;
  unsigned long door = 0;
  unsigned long player = 0;
  unsigned long enemy = 0;
  unsigned long potion = 0;
  unsigned long gold = 0;
  unsigned long stairs = 0;

  unsigned long color(const char* name, unsigned long fallback) const;
  unsigned long cellColor(const Cell& cell) const;
  unsigned long entityColor(const Entity& entity) const;
  void drawText(int x, int y, const std::string& text, unsigned long textColor) const;
  void redraw(const PlayerDisplayInfo* info = nullptr) const;

  virtual void onNotify(const EntityDeathEvent& event) override;
  virtual void onNotify(const EntityMoveEvent& event) override;
  virtual void onNotify(const NewFloorEvent& event) override;

 public:
  X11Renderer();
  virtual ~X11Renderer() override;
  X11Renderer(const X11Renderer&) = delete;
  X11Renderer& operator=(const X11Renderer&) = delete;

  virtual void drawStartScreen() override;
  virtual void draw(const PlayerDisplayInfo& info) override;
  virtual void drawGameOutcome(bool victory, double score) override;
};
