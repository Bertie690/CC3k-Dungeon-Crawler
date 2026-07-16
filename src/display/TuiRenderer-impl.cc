module tui_renderer;

#ifdef __INTELLISENSE__
#include <iostream>
#include "tui_renderer.cc"
#else
import <iostream>;
#endif

TUIRenderer::TUIRenderer() : currentFloor{nullptr} {}

void TUIRenderer::rebuildGrid() {
    if (!currentFloor) return;

    displayGrid.clear();

    displayGrid.resize(Floor::HEIGHT, string(Floor::WIDTH, ' '));

    for (int y = 0; y < Floor::HEIGHT; ++y) {
        for (int x = 0; x < Floor::WIDTH; ++x) {
            Position pos{x, y};

            if (currentFloor->hasCell(pos)) {
                displayGrid[y][x] = determineSymbol(currentFloor->getCell(pos));
            }
            else {
                displayGrid[y][x] = ' ';
            }
        }
    }
}

char TUIRenderer::determineSymbol(const Cell& cell) const {
    if (cell.isOccupied()) {
        return getEntitySymbol(*cell.getEntities().back());
    }
    
    switch (cell.getTileType()) {
        case TileType::Floor: return '.';
        case TileType::VerticalWall: return '|':
        case TileType::HorizontalWall: return '-':
        case TileType::Door: return '+':
        case TileType::Passage: return '#':
        defualt: ' ';
    }
}

char TUIRenderer::getEntitySymbol(const Entity& entity) const noexcept {

    if (auto staircase = dynamic_cast<const Staircase*>(&entity)) {
        return '\\';
    }
    if (auto character = dynamic_cast<Character*>(entity)) {
        switch (character->type) {
            case CharacterType::Player:     return '@';
            case CharacterType::Dragon:    return 'D';
            case CharacterType::Halfling:     return 'L';
            case CharacterType::Merchant:   return 'M';
            case CharacterType::Human:      return 'H';
            case CharacterType::Dwarf:      return 'W';
            case CharacterType::Elf:      return 'E';
            case CharacterType::Orc:      return 'O';
            default:                        return '.';
        }
    }
    if (auto item = dynamic_cast<const Item*>(&entity)) {
        switch (item->type) {
            case ItemType::Potion:  return 'P';
            case ItemType::Gold:    return 'G';
            default:                return '.';
        }
    }
    return '?';
}

void TUIRendeer::draw() {
    if (!currentFloor){
        cout << "No floor loaded" << endl;
    }

    for (const auto& row : displayGrid) {
        std::cout << row << '\n';
    }
}

void TUIRenderer::onNotify(const EntityDeathEvent& event) {
  if (!currentFloor) return;
  const Cell& cell = currentFloor->getCell(event.position);
  displayGrid[event.position.y][event.position.x] = determineSymbol(cell);
}

void TUIRenderer::onNotify(const MoveEvent& event) {
  if (!currentFloor) return;
  
  const Cell& fromCell = currentFloor->getCell(event.from);
  const Cell& toCell = currentFloor->getCell(event.to);
  
  displayGrid[event.from.y][event.from.x] = determineSymbol(fromCell);
  displayGrid[event.to.y][event.to.x] = determineSymbol(toCell);
}

void TUIRenderer::onNotify(const NewFloorEvent& event) {
  currentFloor = event.newFloor;
  rebuildGrid();
}




