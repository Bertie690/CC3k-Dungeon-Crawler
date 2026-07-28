export module compositerenderer;

#pragma once

#ifdef __INTELLISENSE__
#include <memory>
#include <vector>

#include "../events/floor-events.cc"
#include "../events/game-events.cc"
#include "renderer.cc"
#else
import <memory>;
import <vector>;
import floorevents;
import gameevents;
import renderer;
#endif

// Fans rendering events out to each configured display.
export class CompositeRenderer final : public Renderer {
  std::vector<std::unique_ptr<Renderer>> renderers;

  virtual void onNotify(const EntityDeathEvent& event) override;
  virtual void onNotify(const EntityMoveEvent& event) override;
  virtual void onNotify(const NewFloorEvent& event) override;

 public:
  explicit CompositeRenderer(std::vector<std::unique_ptr<Renderer>> renderers);
  virtual void drawStartScreen() override;
  virtual void draw(const PlayerDisplayInfo& info) override;
  virtual void drawGameOutcome(bool victory, double score) override;
};
