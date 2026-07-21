module tuirenderer;

#ifdef __INTELLISENSE__
#include <iostream>

#include "../entities/character.cc"
#include "../entities/entity.cc"
#include "../entities/item.cc"
#include "../entities/staircase.cc"
#include "../enums/item-type.cc"
#include "../enums/race-type.cc"
#include "../enums/tile-type.cc"
#include "../floor/position.cc"
#include "tui-renderer.cc"
#else
import <iostream>;
import staircase;
import entity;
import character;
import item;
import itemtype;
import racetype;
import tiletype;
import position;
#endif  // __INTELLISENSE__

using namespace std;

namespace Color {
  const string RESET = "\033[0m";
  const string BOLD = "\033[1m";
  const string RED = "\033[31m";
  const string GREEN = "\033[32m";
  const string YELLOW = "\033[33m";
  const string BLUE = "\033[34m";
  const string MAGENTA = "\033[35m";
  const string CYAN = "\033[36m";
}  // namespace Color

TUIRenderer::TUIRenderer() : currentFloor{nullptr} {}

// TODO: Send the rendere an event with the finished grid instead of tracking the floor all the time
void TUIRenderer::rebuildGrid() {
  if (!currentFloor) return;

  displayGrid.clear();
  displayGrid.resize(Floor::HEIGHT);

  for (int y = 0; y < Floor::HEIGHT; ++y) {
    displayGrid[y].resize(Floor::WIDTH, " ");
    for (int x = 0; x < Floor::WIDTH; ++x) {
      Position pos{x, y};

      // Add decorative walls around edges
      if (x == 0 || x == Floor::WIDTH - 1) {
        displayGrid[y][x] = "|";  // corners use vertical walls
      } else if (y == 0 || y == Floor::HEIGHT - 1) {
        displayGrid[y][x] = "-";
      } else if (currentFloor->hasCell(pos)) {
        displayGrid[y][x] = getCellText(currentFloor->getCell(pos));
      }
    }
  }
}

string TUIRenderer::getCellText(const Cell& cell) const {
  if (cell.isOccupied()) {
    // TODO: This doesn't always work? We need some sort of precedence
    return getEntitySymbol(*cell.getEntities().back());
  }

  switch (cell.tileType) {
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
    return Color::BLUE + "\\" + Color::RESET;
  }

  if (auto character = dynamic_cast<const Character*>(&entity)) {
    switch (character->raceType()) {
      case RaceType::Human:
        return Color::RED + "H" + Color::RESET;
      case RaceType::Dwarf:
        return Color::RED + "W" + Color::RESET;
      case RaceType::Elf:
        return Color::RED + "E" + Color::RESET;
      case RaceType::Orc:
        return Color::RED + "O" + Color::RESET;
      case RaceType::Merchant:
        return Color::RED + "M" + Color::RESET;
      case RaceType::Dragon:
        return Color::RED + "D" + Color::RESET;
      case RaceType::Halfling:
        return Color::RED + "L" + Color::RESET;
      case RaceType::Shade:
      case RaceType::Drow:
      case RaceType::Vampire:
      case RaceType::Troll:
      case RaceType::Goblin:
        return Color::RED + "@" + Color::RESET;
      default:
        throw std::runtime_error("Unknown character race type!");
    }
  }

  if (auto item = dynamic_cast<const Item*>(&entity)) {
    switch (item->type) {
      case ItemType::Potion:
        return Color::GREEN + "P" + Color::RESET;
      case ItemType::GoldPile:
        return Color::YELLOW + "G" + Color::RESET;
      default:
        throw std::runtime_error("Unknown item type");
    }
  }
  throw std::runtime_error("Unknown entity class instance!");
}

void TUIRenderer::draw() {
  if (!currentFloor) {
    // TODO: Add race selection screen?
    cerr << "No floor loaded" << endl;
    return;
  }

  for (const vector<string>& row : displayGrid) {
    for (const string& cell : row) {
      cout << cell;
    }
    cout << endl;
  }
}

void TUIRenderer::onNotify(const EntityDeathEvent& event) {
  if (!currentFloor) return;
  const Position position = event.entity.position();
  const Cell& cell = currentFloor->getCell(position);
  displayGrid[position.y][position.x] = getCellText(cell);
}

void TUIRenderer::onNotify(const EntityMoveEvent& event) {
  if (!currentFloor) return;

  const Cell& fromCell = currentFloor->getCell(event.from);
  const Cell& toCell = currentFloor->getCell(event.to);

  displayGrid[event.from.y][event.from.x] = getCellText(fromCell);
  displayGrid[event.to.y][event.to.x] = getCellText(toCell);
}

void TUIRenderer::onNotify(const NewFloorEvent& event) {
  currentFloor = event.newFloor;
  rebuildGrid();
}
