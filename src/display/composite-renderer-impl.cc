module compositerenderer;

#ifdef __INTELLISENSE__
#include "composite-renderer.cc"
#else
import renderer;
#endif

using namespace std;

CompositeRenderer::CompositeRenderer(vector<unique_ptr<Renderer>> renderers)
    : renderers{move(renderers)} {}

void CompositeRenderer::drawStartScreen() {
  for (const auto& renderer : renderers) renderer->drawStartScreen();
}

void CompositeRenderer::draw(const PlayerDisplayInfo& info) {
  for (const auto& renderer : renderers) renderer->draw(info);
}

void CompositeRenderer::drawGameOutcome(bool victory, double score) {
  for (const auto& renderer : renderers) renderer->drawGameOutcome(victory, score);
}

void CompositeRenderer::onNotify(const EntityDeathEvent& event) {
  for (const auto& renderer : renderers) renderer->onEntityDeath(event);
}

void CompositeRenderer::onNotify(const EntityMoveEvent& event) {
  for (const auto& renderer : renderers) renderer->onEntityMove(event);
}

void CompositeRenderer::onNotify(const NewFloorEvent& event) {
  for (const auto& renderer : renderers) renderer->onNewFloor(event);
}
