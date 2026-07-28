module;
#include <X11/Xlib.h>

module x11renderer;

#ifdef __INTELLISENSE__
#include <stdexcept>

#include "../entities/character.cc"
#include "../entities/item.cc"
#include "../entities/staircase.cc"
#include "../enums/item-type.cc"
#include "../enums/race-type.cc"
#include "../floor/cell.cc"
#include "../floor/floor.cc"
#include "x11-renderer.cc"
#else
import <stdexcept>;
import character;
import item;
import itemtype;
import racetype;
import staircase;
import cell;
import floor;
import position;
import tiletype;
import racetype;
#endif

using namespace std;

namespace {
  constexpr int CELL_SIZE = 12;
  constexpr int MAP_X = 16;
  constexpr int MAP_Y = 32;
  constexpr int INFO_X = MAP_X + Floor::WIDTH * CELL_SIZE + 24;
  constexpr int WINDOW_WIDTH = INFO_X + 260;
  constexpr int WINDOW_HEIGHT = MAP_Y + Floor::HEIGHT * CELL_SIZE + 24;
}  // namespace

X11Renderer::X11Renderer() {
  Display* xDisplay = XOpenDisplay(nullptr);
  if (!xDisplay) throw runtime_error("Unable to open an X11 display");

  display = xDisplay;
  screen = DefaultScreen(xDisplay);
  black = BlackPixel(xDisplay, screen);
  white = WhitePixel(xDisplay, screen);
  wall = color("gray35", white);
  floor = color("gray15", black);
  passage = color("goldenrod4", wall);
  door = color("sienna3", wall);
  player = color("deepskyblue", white);
  enemy = color("firebrick2", white);
  potion = color("medium sea green", white);
  gold = color("gold", white);
  stairs = color("plum", white);

  Window xWindow = XCreateSimpleWindow(xDisplay, RootWindow(xDisplay, screen), 0, 0, WINDOW_WIDTH,
                                       WINDOW_HEIGHT, 0, black, black);
  window = xWindow;
  XStoreName(xDisplay, xWindow, "CC3K graphical display");
  XSelectInput(xDisplay, xWindow, ExposureMask | StructureNotifyMask);
  XMapRaised(xDisplay, xWindow);
  graphicsContext = XCreateGC(xDisplay, xWindow, 0, nullptr);
  XFlush(xDisplay);
}

X11Renderer::~X11Renderer() {
  Display* xDisplay = static_cast<Display*>(display);
  if (!xDisplay) return;
  XFreeGC(xDisplay, static_cast<GC>(graphicsContext));
  XDestroyWindow(xDisplay, static_cast<Window>(window));
  XCloseDisplay(xDisplay);
}

unsigned long X11Renderer::color(const char* name, unsigned long fallback) const {
  XColor exact;
  XColor closest;
  Display* xDisplay = static_cast<Display*>(display);
  return XAllocNamedColor(xDisplay, DefaultColormap(xDisplay, screen), name, &closest, &exact)
             ? closest.pixel
             : fallback;
}

unsigned long X11Renderer::cellColor(const Cell& cell) const {
  switch (cell.tileType) {
    case TileType::Floor:
      return floor;
    case TileType::VerticalWall:
    case TileType::HorizontalWall:
      return wall;
    case TileType::Door:
      return door;
    case TileType::Passage:
      return passage;
  }
  return black;
}

unsigned long X11Renderer::entityColor(const Entity& entity) const {
  if (dynamic_cast<const Staircase*>(&entity)) return stairs;
  if (const auto* character = dynamic_cast<const Character*>(&entity)) {
    return isPlayer(character->raceType()) ? player : enemy;
  }
  if (const auto* item = dynamic_cast<const Item*>(&entity)) {
    return item->type == ItemType::Potion ? potion : gold;
  }
  return white;
}

string X11Renderer::entitySymbol(const Entity& entity) const {
  if (dynamic_cast<const Staircase*>(&entity)) return "\\";
  if (const auto* character = dynamic_cast<const Character*>(&entity)) {
    if (isPlayer(character->raceType())) return "@";
    switch (character->raceType()) {
      case RaceType::Human:
        return "H";
      case RaceType::Dwarf:
        return "W";
      case RaceType::Elf:
        return "E";
      case RaceType::Orc:
        return "O";
      case RaceType::Merchant:
        return "M";
      case RaceType::Dragon:
        return "D";
      case RaceType::Halfling:
        return "L";
      default:
        return "?";
    }
  }
  if (const auto* item = dynamic_cast<const Item*>(&entity)) {
    return item->type == ItemType::Potion ? "P" : "G";
  }
  return "?";
}

void X11Renderer::drawText(int x, int y, const string& text, unsigned long textColor) const {
  Display* xDisplay = static_cast<Display*>(display);
  GC gc = static_cast<GC>(graphicsContext);
  XSetForeground(xDisplay, gc, textColor);
  XDrawString(xDisplay, static_cast<Window>(window), gc, x, y, text.c_str(), text.size());
}

void X11Renderer::redraw(const PlayerDisplayInfo* info) const {
  Display* xDisplay = static_cast<Display*>(display);
  GC gc = static_cast<GC>(graphicsContext);
  XClearWindow(xDisplay, static_cast<Window>(window));
  if (!currentFloor) {
    drawText(MAP_X, MAP_Y, "Choose a race in the terminal to begin.", white);
    XFlush(xDisplay);
    return;
  }

  for (int y = 0; y < Floor::HEIGHT; ++y) {
    for (int x = 0; x < Floor::WIDTH; ++x) {
      const Position position{x, y};
      if (!currentFloor->hasCell(position)) continue;
      const Cell& cell = currentFloor->getCell(position);
      XSetForeground(xDisplay, gc, cellColor(cell));
      XFillRectangle(xDisplay, static_cast<Window>(window), gc, MAP_X + x * CELL_SIZE,
                     MAP_Y + y * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1);
      if (!cell.getEntities().empty()) {
        const Entity& entity = *cell.getEntities().back();
        const string symbol = entitySymbol(entity);
        XSetForeground(xDisplay, gc, entityColor(entity));
        XDrawString(xDisplay, static_cast<Window>(window), gc, MAP_X + x * CELL_SIZE + 3,
                    MAP_Y + y * CELL_SIZE + 9, symbol.c_str(), symbol.size());
      }
    }
  }

  if (info) {
    drawText(INFO_X, MAP_Y, "CC3K", white);
    drawText(INFO_X, MAP_Y + 28, "Gold: " + to_string(info->gold), gold);
    drawText(INFO_X, MAP_Y + 48, "Floor: " + to_string(info->floorNumber), white);
    drawText(INFO_X, MAP_Y + 68, "HP: " + to_string(info->hp), white);
    drawText(INFO_X, MAP_Y + 88, "Atk: " + to_string(info->stats.atk), white);
    drawText(INFO_X, MAP_Y + 108, "Def: " + to_string(info->stats.def), white);
    drawText(INFO_X, MAP_Y + 138, info->action, white);
  }
  XFlush(xDisplay);
}

void X11Renderer::drawStartScreen() { redraw(); }

void X11Renderer::draw(const PlayerDisplayInfo& info) { redraw(&info); }

void X11Renderer::drawGameOutcome(bool victory, double score) {
  const string outcome = victory ? "Victory! Score: " + to_string(score) : "Game over";
  drawText(INFO_X, WINDOW_HEIGHT - 20, outcome, victory ? gold : enemy);
  XFlush(static_cast<Display*>(display));
}

void X11Renderer::onNotify(const EntityDeathEvent&) {}
void X11Renderer::onNotify(const EntityMoveEvent&) {}
void X11Renderer::onNotify(const NewFloorEvent& event) { currentFloor = event.newFloor; }
