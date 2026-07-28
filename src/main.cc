#ifdef __INTELLISENSE__
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "display/composite-renderer.cc"
#include "display/renderer.cc"
#include "display/tui-renderer.cc"
#include "display/x11-renderer.cc"
#include "enums/action.cc"
#include "enums/direction.cc"
#include "enums/race-type.cc"
#include "game/feature-flags.cc"
#include "game/game.cc"
#include "input/input-handler.cc"
#include "input/stdin-input-handler.cc"
#include "utils/colors.cc"
#else
import <iostream>;
import <memory>;
import <stdexcept>;
import <string>;
import <vector>;
import <fstream>;
import <cstdlib>;
import tuirenderer;
import compositerenderer;
import renderer;
import x11renderer;
import action;
import direction;
import racetype;
import game;
import stdininputhandler;
import inputhandler;
import colors;
import featureflags;
#endif  // __INTELLISENSE__

using namespace std;

void printHelpMessage(string prog_name = "./cc3k") {
  // clang-format off

  const string msg =
      "CC3K\n"
      "A simple rogue-like dungeon crawler implemented in C++.\n"
      "Usage: " + prog_name + " [options] [floor_file]\n"
      "Options:\n"
      "  --help                      Show this help message and exit.\n"
      "  --version                   Show the version information and exit.\n"
      "  --seed=<number>             Set the random seed for the game.\n"
      "  --debug                     Enable debug mode (spawns on floor 5).\n"
      "  --enhanced-graphics         Open an X11 graphical display alongside the terminal display.\n"
      "  --improved-hoard-resolution Enable the improved dragon hoard resolution algorithm for preset floors.\n"
      "  --numpad                    Enable numeric keypad movement (1-4, 6-9).\n"
      "Arguments:\n"
      "  floor_file                  Optional path to a preset floor file to load.\n";
  // clang-format on
  cout << msg << endl;
}

string parseFloorFile(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] != '-') {
      return string(argv[i]);
    }
  }
  return "";
}

unsigned int parseSeed(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg.find("--seed=") == 0) {
      string seedStr = arg.substr(7);
      if (seedStr.empty()) {
        cerr << "Invalid seed value: (empty)" << endl;
        return 0;
      }

      unsigned int result = 0;
      for (char c : seedStr) {
        if (c < '0' || c > '9') {
          cerr << "Invalid seed value: " << seedStr << endl;
          return 0;
        }
        result = result * 10 + static_cast<unsigned int>(c - '0');
      }
      return result;
    }
  }
  return 0;
}

void parseFeatureFlags(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "--debug") {
      FeatureFlags::enableDebugMode = true;
    } else if (arg == "--enhanced-graphics") {
      FeatureFlags::enableEnhancedGraphics = true;
    } else if (arg == "--improved-hoard-resolution") {
      FeatureFlags::useImprovedHoardResolution = true;
    } else if (arg == "--numpad") {
      FeatureFlags::enableNumpadInput = true;
    }
  }
}

int main(int argc, char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "--help") {
      printHelpMessage(argv[0]);
      return 0;
    }
  }

  string floorFile = parseFloorFile(argc, argv);
  unsigned int seed = parseSeed(argc, argv);
  parseFeatureFlags(argc, argv);

  if (FeatureFlags::enableDebugMode) {
    cout << "Debug mode enabled" << endl;
  }

  if (FeatureFlags::enableEnhancedGraphics) {
    cout << "Enhanced graphics enabled" << endl;
  }

  if (FeatureFlags::useImprovedHoardResolution) {
    cout << "Improved dragon hoard resolution algorithm enabled" << endl;
  }

  if (FeatureFlags::enableNumpadInput) {
    cout << "Numpad movement enabled" << endl;
  }

  unique_ptr<InputHandler> inputHandler = make_unique<StdinInputHandler>();
  unique_ptr<Renderer> renderer;
  if (FeatureFlags::enableEnhancedGraphics) {
    vector<unique_ptr<Renderer>> displays;
    displays.emplace_back(make_unique<TUIRenderer>());
    displays.emplace_back(make_unique<X11Renderer>());
    renderer = make_unique<CompositeRenderer>(move(displays));
  } else {
    renderer = make_unique<TUIRenderer>();
  }

  // Create game with appropriate parameters
  unique_ptr<Game> game;
  if (seed == 0) {
    game = make_unique<Game>(move(renderer), floorFile);
  } else {
    game = make_unique<Game>(move(renderer), floorFile, seed);
  }

  inputHandler->attach(game.get());
  game->attach(inputHandler.get());

  // Main game loop using InputHandler
  bool running = true;
  while (running) {
    try {
      running = inputHandler->processInput();
    } catch (const invalid_argument& e) {
      cerr << "Input error: " << e.what() << endl;
    } catch (const out_of_range& e) {
      cerr << "Input error: " << e.what() << endl;
    } catch (const exception& e) {
      cerr << "Uncaught exception while running game: \n" << Color::colorize(Color::RED, e.what())
           << endl;
      return 1;
    }
  }
  return 0;
}
