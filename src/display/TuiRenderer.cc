export module tui_renderer;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>
#include <string>
#include <renderer.cc>
#include "observer.cc"
#include "game_events.cc"
#include "floor.cc"
#else
import renderer;
import observer;
imprt game_events;
import floor;
import <vector>;
import <string>;
#endif

using namespace std;

export class TUIRenderer : public Renderer, public Observer<EntityDeathEvent, MoveEvent, NewFloorEvent> {
    const Floor* currentFloor;
    vector<string> displayGrid;

    char determineSymbol(const Cell& cell) const;
    char getEntitySymbol(const Entity& entity) const;
    void rebuildGrid();

    public:
     TUIRenderer();
     void draw override;
     void OnNotify(const EntityDeathEvent& event) override;
     void OnNotify(const MoveEvent& event) override;
     void OnNotify(const NewFloorEven& event) override;
};

