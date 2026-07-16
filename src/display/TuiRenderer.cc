export module tui_render;

#pragma once

#ifdef __INTELLISENSE__
#include <vector>
#include <string>
#include <render.cc>
#include "observer.cc"
#include "game_events.cc"
#include "floor.cc"
#else
import render;
import observer;
imprt game_events;
import floor;
import <vector>;
import <string>;
#endif

using namespace std;

export class TUIRender : public Render, public Observer<EntityDeathEvent, MoveEvent, NewFloorEvent> {
    const Floor* currentFloor;
    vector<string> displayGrid;

    char determineSymbol(const Cell& cell) const;
    void rebuildGrid();

    public:
     TuiRender();
     void draw override;
     void OnNotify(const EntityDeathEvent& event) override;
     void OnNotify(const MoveEvent& event) override;
     void OnNotify(const NewFloorEven& event) override;
};

