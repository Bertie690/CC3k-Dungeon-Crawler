module tui_render;

#ifdef __INTELLISENSE__
#include <iostream>
#include "tui_render.cc"
#else
import <iostream>;
#endif

TUIRender::TUIRender() : currentFloor{nullptr} {}

void TUIRender::rebuildGrid() {
    if (!currentFloor) return;

    displayGrid = vector<string>(currentFloor->getHeight(), currentFloor->getWidth(), ' ');

    for (int y = 0; y < currentFloor->getHeight(); ++y) {
        for (int x = 0; x < currentFloor->getWidth(); ++x) {
            Position pos{x, y};
            displayGrid[y][x] = determineSymbol(currentFloor->getCell(pos));
        }
    }
}

