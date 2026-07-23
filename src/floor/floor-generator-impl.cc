module floorgenerator;

#ifdef __INTELLISENSE__
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "../enums/tile-type.cc"
#include "cell.cc"
#include "chamber.cc"
#include "floor-generator.cc"
#include "floor.cc"
#include "passage.cc"
#include "position.cc"
#else
import <memory>;
import <string>;
import <utility>;
import <vector>;
import tiletype;
import cell;
import chamber;
import floor;
import passage;
import position;
#endif  // __INTELLISENSE__

using namespace std;

// TODO: potential bonus handling any floor layout with flood fill

// fixed terrain layout (emptyfloor.txt)
const vector<string> BASE_TERRAIN{
    "|-----------------------------------------------------------------------------|",
    "|                                                                             |",
    "| |--------------------------|        |-----------------------|               |",
    "| |..........................|        |.......................|               |",
    "| |..........................+########+.......................|-------|       |",
    "| |..........................|   #    |...............................|--|    |",
    "| |..........................|   #    |..................................|--| |",
    "| |----------+---------------|   #    |----+----------------|...............| |",
    "|            #                 #############                |...............| |",
    "|            #                 #     |-----+------|         |...............| |",
    "|            #                 #     |............|         |...............| |",
    "|            ###################     |............|   ######+...............| |",
    "|            #                 #     |............|   #     |...............| |",
    "|            #                 #     |-----+------|   #     |--------+------| |",
    "|  |---------+-----------|     #           #          #              #        |",
    "|  |.....................|     #           #          #         |----+------| |",
    "|  |.....................|     ########################         |...........| |",
    "|  |.....................|     #           #                    |...........| |",
    "|  |.....................|     #    |------+--------------------|...........| |",
    "|  |.....................|     #    |.......................................| |",
    "|  |.....................+##########+.......................................| |",
    "|  |.....................|          |.......................................| |",
    "|  |---------------------|          |---------------------------------------| |",
    "|                                                                             |",
    "|-----------------------------------------------------------------------------|"};

// map that describes which Room owns each Cell
// 1-5 Chambers, A-B Passages and spaces mean no Cell exists
const vector<string> ROOM_OWNERS{
    "                                                                               ",
    "                                                                               ",
    "  1111111111111111111111111111        2222222222222222222222222                ",
    "  1111111111111111111111111111        2222222222222222222222222                ",
    "  1111111111111111111111111111AAAAAAAA222222222222222222222222222222222        ",
    "  1111111111111111111111111111   A    222222222222222222222222222222222222     ",
    "  1111111111111111111111111111   A    222222222222222222222222222222222222222  ",
    "  1111111111111111111111111111   A    222222222222222222222222222222222222222  ",
    "             A                 AAAAAAAAAAAAA                22222222222222222  ",
    "             A                 A     33333333333333         22222222222222222  ",
    "             A                 A     33333333333333         22222222222222222  ",
    "             AAAAAAAAAAAAAAAAAAA     33333333333333   AAAAAA22222222222222222  ",
    "             A                 A     33333333333333   A     22222222222222222  ",
    "             A                 A     33333333333333   A     22222222222222222  ",
    "   44444444444444444444444     A           A          A              B         ",
    "   44444444444444444444444     A           A          A         5555555555555  ",
    "   44444444444444444444444     AAAAAAAAAAAAAAAAAAAAAAAA         5555555555555  ",
    "   44444444444444444444444     A           A                    5555555555555  ",
    "   44444444444444444444444     A    55555555555555555555555555555555555555555  ",
    "   44444444444444444444444     A    55555555555555555555555555555555555555555  ",
    "   44444444444444444444444AAAAAAAAAA55555555555555555555555555555555555555555  ",
    "   44444444444444444444444          55555555555555555555555555555555555555555  ",
    "   44444444444444444444444          55555555555555555555555555555555555555555  ",
    "                                                                               ",
    "                                                                               "};

TileType getTileType(char symbol) {
  if (symbol == '|') return TileType::VerticalWall;
  if (symbol == '-') return TileType::HorizontalWall;
  if (symbol == '+') return TileType::Door;
  if (symbol == '#') return TileType::Passage;
  return TileType::Floor; // symbol == '.'
}

Floor FloorGenerator::createBaseFloor(RNG& rng) {
  Floor floor{rng};

  vector<vector<unique_ptr<Cell>>> chamberCells(5);
  vector<vector<unique_ptr<Cell>>> passageCells(2);

  for (int y = 0; y < Floor::HEIGHT; ++y) {
    for (int x = 0; x < Floor::WIDTH; ++x) {
      const char owner = ROOM_OWNERS[y][x];
      if (owner == ' ') continue;

      unique_ptr<Cell> cell = make_unique<Cell>(Position{x, y}, getTileType(BASE_TERRAIN[y][x]));

      if ('1' <= owner && owner <= '5') chamberCells[owner - '1'].push_back(move(cell));
      else passageCells[owner - 'A'].push_back(move(cell));
    }
  }
  for (vector<unique_ptr<Cell>>& cells : chamberCells) {
    floor.addRoom(make_unique<Chamber>(move(cells)));
  }
  for (vector<unique_ptr<Cell>>& cells : passageCells) {
    floor.addRoom(make_unique<Passage>(move(cells)));
  }
  return floor;
}
