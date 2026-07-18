#ifdef __INTELLISENSE__
#include <memory>

#include "display/tui-renderer.cc"
#include "game/game.cc"
#else
import <memory>;
import tuirenderer;
import game;
#endif  // __INTELLISENSE__

using namespace std;

int main() {
  Game game{make_unique<TUIRenderer>()};
  game.newGame();
}
