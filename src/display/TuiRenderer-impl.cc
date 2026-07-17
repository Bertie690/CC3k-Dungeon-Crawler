module tui_renderer;

#ifdef __INTELLISENSE__
#include <iostream>

#include "tui_renderer.cc"
#else
import <iostream>;
#endif

using namespace std;

namespace Color {
  const string Reset = "\033[0m";
  const string Bold = "\033[1m";
  const string Red = "\033[31m";
  const string Green = "\033[32m";
  const string Yellow = "\033[33m";
  const string Blue = "\033[34m";
  const string Magenta = "\033[35m";
  const string Cyan = "\033[36m";
}  // namespace Color

TUIRenderer::TUIRenderer() : currentFloor{nullptr} {}

void TUIRenderer::rebuildGrid() {
  if (!currentFloor) return;

  displayGrid.clear();
  displayGrid.resize(Floor::HEIGHT);

  for (int y = 0; y < Floor::HEIGHT; ++y) {
    displayGrid[y].resize(Floor::WIDTH, " ");
    for (int x = 0; x < Floor::WIDTH; ++x) {
      Position pos{x, y};

      if (currentFloor->hasCell(pos)) {
        displayGrid[y][x] = determineSymbol(currentFloor->getCell(pos));
      } else {
        displayGrid[y][x] = " ";
      }
    }
  }
}

string TUIRenderer::determineSymbol(const Cell& cell) const {
  if (cell.isOccupied()) {
    return getEntitySymbol(*cell.getEntities().back());
  }

  switch (cell.getTileType()) {
    case TileType::Floor:
      return ".";
    case TileType::VerticalWall:
      return "|";
    case TileType::HorizontalWall:
      return "-";
    case TileType::Door:
      return "+";
    case TileType::Passage:
      return "#";
    default:
      return " ";
  }
}

string TUIRenderer::getEntitySymbol(const Entity& entity) const noexcept {
  if (auto staircase = dynamic_cast<const Staircase*>(&entity)) {
    return Color::Blue + "\\" + Color::Reset;
  }
  if (auto character = dynamic_cast<const Character*>(&entity)) {
    switch (character->type) {
      case CharacterType::Player:
        return Color::Blue + "@" + Color::Reset;
      case CharacterType::Dragon:
        return Color::Red + "D" + Color::Reset;
      case CharacterType::Halfling:
        return Color::Red + "L" + Color::Reset;
      case CharacterType::Merchant:
        return Color::Red + "M" + Color::Reset;
      case CharacterType::Human:
        return Color::Red + "H" + Color::Reset;
      case CharacterType::Dwarf:
        return Color::Red + "W" + Color::Reset;
      case CharacterType::Elf:
        return Color::Red + "E" + Color::Reset;
      case CharacterType::Orc:
        return Color::Red + "O" + Color::Reset;
      default:
        return ".";
    }
  }
  if (auto item = dynamic_cast<const Item*>(&entity)) {
    switch (item->type) {
      case ItemType::Potion:
        return Color::Green + "P" + Color::Reset;
      case ItemType::Gold:
        return Color::Yellow + "G" + Color::Reset;
      default:
        return ".";
    }
  }
  return '?';
}

void TUIRenderer::draw() {
  if (!currentFloor) {
    cout << "No floor loaded" << endl;
    return;
  }

  for (const auto& row : displayGrid) {
    for (const auto& cell : row) {
      cout << cell;
    }
    cout << endl;
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
