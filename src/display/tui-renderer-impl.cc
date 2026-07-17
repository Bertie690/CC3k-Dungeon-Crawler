module tuirenderer;

#ifdef __INTELLISENSE__
#include <iostream>

#include "../entities/character.cc"
#include "../entities/entity.cc"
#include "../entities/item.cc"
#include "../entities/staircase.cc"
#include "../enums/item-type.cc"
#include "tui-renderer.cc"
#else
import <iostream>;
import staircase;
import entity;
import character;
import item;
import itemtype;

#endif  // __INTELLISENSE__

using namespace std;

TUIRenderer::TUIRenderer() : currentFloor{nullptr} {}

// TODO: Send the rendere an event with the finished grid instead of tracking the floor all the time
void TUIRenderer::rebuildGrid() {
  if (!currentFloor) return;

  displayGrid.clear();

  displayGrid.resize(Floor::HEIGHT, string(Floor::WIDTH, ' '));

  for (int y = 0; y < Floor::HEIGHT; ++y) {
    for (int x = 0; x < Floor::WIDTH; ++x) {
      Position pos{x, y};

      if (currentFloor->hasCell(pos)) {
        displayGrid[y][x] = determineSymbol(currentFloor->getCell(pos));
      } else {
        displayGrid[y][x] = ' ';
      }
    }
  }
}

// TODO: This needs to return a string for coloration
char TUIRenderer::determineSymbol(const Cell& cell) const {
  if (cell.isOccupied()) {
    // TODO: This doesn't always work? We need some sort of precedence
    return getEntitySymbol(*cell.getEntities().back());
  }

  switch (cell.getTileType()) {
    case TileType::Floor:
      return '.';
    case TileType::VerticalWall:
      return '|';
    case TileType::HorizontalWall:
      return '-';
    case TileType::Door:
      return '+';
    case TileType::Passage:
      return '#';
    default:
      return ' ';
  }
}

char TUIRenderer::getEntitySymbol(const Entity& entity) const {
  if (auto staircase = dynamic_cast<const Staircase*>(&entity)) {
    return '\\';
  }

  if (auto character = dynamic_cast<const Character*>(&entity)) {
    switch (character->raceType) {
      case RaceType::Human:
        return 'H';
      case RaceType::Dwarf:
        return 'W';
      case RaceType::Elf:
        return 'E';
      case RaceType::Orc:
        return 'O';
      case RaceType::Merchant:
        return 'M';
      case RaceType::Dragon:
        return 'D';
      case RaceType::Halfling:
        return 'L';
      case RaceType::Shade:
      case RaceType::Drow:
      case RaceType::Vampire:
      case RaceType::Troll:
      case RaceType::Goblin:
        return '@';
      default:
        throw std::runtime_error("Unknown character type");
    }
  }

  if (auto item = dynamic_cast<const Item*>(&entity)) {
    switch (item->type) {
      case ItemType::Potion:
        return 'P';
      case ItemType::GoldPile:
        return 'G';
      default:
        throw std::runtime_error("Unknown item type");
    }
  }
  throw std::runtime_error("Unknown entity class instance!");
  return '?';
}

void TUIRenderer::draw() {
  if (!currentFloor) {
    cerr << "No floor loaded" << endl;
    return;
  }

  for (const auto& row : displayGrid) {
    std::cout << row << '\n';
  }
}

void TUIRenderer::onNotify(const EntityDeathEvent& event) {
  if (!currentFloor) return;
  const Position position = event.entity.position;
  const Cell& cell = currentFloor->getCell(position);
  displayGrid[position.y][position.x] = determineSymbol(cell);
}

void TUIRenderer::onNotify(const EntityMoveEvent& event) {
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
