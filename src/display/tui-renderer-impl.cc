module tuirenderer;

#ifdef __INTELLISENSE__
#include <iostream>
#include <limits>
#include <sstream>

#include "../entities/character.cc"
#include "../entities/entity.cc"
#include "../entities/item.cc"
#include "../entities/staircase.cc"
#include "../enums/item-type.cc"
#include "../enums/race-type.cc"
#include "../enums/tile-type.cc"
#include "../floor/position.cc"
#include "../utils/colors.cc"
#include "tui-renderer.cc"
#else
import <iostream>;
import <sstream>;
import <limits>;
import staircase;
import entity;
import character;
import item;
import itemtype;
import racetype;
import tiletype;
import position;
import colors;
#endif  // __INTELLISENSE__

using namespace std;

namespace {
  void clearConsole() { std::cout << "\033[2J\033[1;1H"; }

  string raceName(RaceType race) {
    switch (race) {
      case RaceType::Shade:
        return "Shade";
      case RaceType::Drow:
        return "Drow";
      case RaceType::Vampire:
        return "Vampire";
      case RaceType::Troll:
        return "Troll";
      case RaceType::Goblin:
        return "Goblin";
      case RaceType::Human:
        return "Human";
      case RaceType::Dwarf:
        return "Dwarf";
      case RaceType::Elf:
        return "Elf";
      case RaceType::Orc:
        return "Orc";
      case RaceType::Merchant:
        return "Merchant";
      case RaceType::Dragon:
        return "Dragon";
      case RaceType::Halfling:
        return "Halfling";
    }
    return "Unknown";
  }
}  // namespace

TUIRenderer::TUIRenderer() : currentFloor{nullptr} {}

void TUIRenderer::drawStartScreen() {
  clearConsole();
  cout << Color::CYAN << Color::BOLD;
  cout << "  ================================================\n";
  cout << "             C C 3 K   D U N G E O N\n";
  cout << "  ================================================" << Color::RESET << "\n\n";
  cout << "  Escape the dungeon by reaching the stairs on floor 5.\n\n";
  cout << Color::YELLOW << "  Choose your adventurer" << Color::RESET << "\n";
  cout << "    s  Shade     d  Drow     v  Vampire\n";
  cout << "    g  Goblin    t  Troll\n\n";
  cout << "  Enter q at any prompt to leave the game.\n\n";
}

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
  if (!cell.getEntities().empty()) {
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
        return Color::CYAN + "@" + Color::RESET;
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

void TUIRenderer::draw(const PlayerDisplayInfo& info) {
  if (!currentFloor) {
    // TODO: Add race selection screen?
    cerr << "No floor loaded" << endl;
    return;
  }

  clearConsole();
  for (const vector<string>& row : displayGrid) {
    for (const string& cell : row) {
      cout << cell;
    }
    cout << endl;
  }

  cout << endl << buildStatsFooter(info);
}

void TUIRenderer::drawGameOutcome(bool victory, double score) {
  clearConsole();
  cout << "\n";
  if (victory) {
    cout << Color::GREEN << Color::BOLD;
    cout << "  ================================================\n";
    cout << "                   V I C T O R Y\n";
    cout << "  ================================================" << Color::RESET << "\n\n";
    cout << "  You escaped the dungeon and passed floor 5.\n";
    cout << "  Final score: " << Color::YELLOW << score << Color::RESET << "\n";
  } else {
    cout << Color::RED << Color::BOLD;
    cout << "  ================================================\n";
    cout << "                   G A M E  O V E R\n";
    cout << "  ================================================" << Color::RESET << "\n\n";
    cout << "  Your adventurer has fallen in the dungeon.\n";
  }
  cout << "\n  Enter " << Color::YELLOW << "r" << Color::RESET << " to play again or "
       << Color::YELLOW << "q" << Color::RESET << " to quit.\n\n";
}

string TUIRenderer::buildStatsFooter(const PlayerDisplayInfo& info) const {
  ostringstream out;
  const std::string maxHpStr = info.stats.maxHp == std::numeric_limits<unsigned int>::max()
                                   ? "∞"
                                   : std::to_string(info.stats.maxHp);
  out << "Race: " << raceName(info.race) << "   Gold: " << info.gold << "   Floor "
      << info.floorNumber << "\n";
  out << "HP: " << info.hp << "/" << maxHpStr << "\n";
  out << "Atk: " << info.stats.atk << "\n";
  out << "Def: " << info.stats.def << "\n";
  out << "Action: " << info.action << "\n";
  return out.str();
}

void TUIRenderer::onNotify(const EntityDeathEvent& event) {
  if (!currentFloor) return;
  const Cell& cell = currentFloor->getCell(event.position);
  displayGrid[event.position.y][event.position.x] = getCellText(cell);
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
