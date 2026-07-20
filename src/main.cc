#ifdef __INTELLISENSE__
#include <iostream>
#include <memory>
#include <string>

#include "display/tui-renderer.cc"
#include "enums/action.cc"
#include "enums/direction.cc"
#include "game/game.cc"
#else
import <iostream>;
import <memory>;
import <string>;
import tuirenderer;
import game;
import action;
import direction;
#endif  // __INTELLISENSE__

using namespace std;

int main() {
  Game game{make_unique<TUIRenderer>()};
  game.newGame();

  // TODO real input handler
  // Temp: test input handler
  char input;
  while (cin >> input) {
    try {
      if (input == 'w') {
        game.runPlayerTurn(Move{Direction::North});
      } else if (input == 'a') {
        game.runPlayerTurn(Move{Direction::West});
      } else if (input == 's') {
        game.runPlayerTurn(Move{Direction::South});
      } else if (input == 'd') {
        game.runPlayerTurn(Move{Direction::East});
      } else {
        continue;
      }
    } catch (const exception& e) {
      cerr << e.what() << endl;
    }
  }
}
