#ifdef __INTELLISENSE__
#include <iostream>
#include <memory>
#include <string>

#include "display/tui-renderer.cc"
#include "game/game.cc"
#else
import <iostream>;
import <memory>;
import <string>;
import tuirenderer;
import game;
#endif  // __INTELLISENSE__

using namespace std;

int main() {
  Game game{make_unique<TUIRenderer>()};
  game.newGame();

  string input;
  while (getline(cin, input)) game.runEnemyTurn();
}
