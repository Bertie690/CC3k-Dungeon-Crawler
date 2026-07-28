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
      "A simple rogue-like dungeon crawler implemented in C++, inspired by Rogue and Hack.\n"
      "Usage: " + prog_name + " [options] [floor_file]\n"
      "Options:\n"
      "  --help                      Show this help message and exit.\n"
      "  --version                   Show version information and exit.\n"
      "  --seed=<number>             Set the initial random seed for the game. Set to 0 to disable.\n"
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
    if (arg.find("--seed=") != 0) {
      continue;
    }
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
  static const string VERSION = "1.0.0";

  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "--help") {
      printHelpMessage(argv[0]);
      return 0;
    }
    if (arg == "--version") {
      cout << "CC3K version " << VERSION << endl;
      return 0;
    }
  }

  string floorFile = parseFloorFile(argc, argv);
  unsigned int seed = parseSeed(argc, argv);
  parseFeatureFlags(argc, argv);

  bool flush = false;
  if (seed != 0) {
    cout << "Initial seed set to " << seed << "!\n";
    flush = true;
  }
  if (FeatureFlags::enableDebugMode) {
    cout << "Debug mode enabled!\n";
    flush = true;
  }
  if (FeatureFlags::enableEnhancedGraphics) {
    cout << "Enhanced graphics enabled!\n";
    flush = true;
  }
  if (FeatureFlags::useImprovedHoardResolution) {
    cout << "Improved dragon hoard resolution algorithm enabled!\n";
    flush = true;
  }
  if (FeatureFlags::enableNumpadInput) {
    cout << "Numpad movement enabled!\n";
    flush = true;
  }
  if (flush) {
    cout.flush();
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
    // This error handling could make a clearer distinction between recoverable and unrecoverable errors,
    // but if it works it works...
    try {
      running = inputHandler->processInput();
    } catch (const invalid_argument& e) {
      cerr << "Input error: " << colorize(Color::RED, e.what()) << endl;
    } catch (const out_of_range& e) {
      cerr << "Input error: " << colorize(Color::RED, e.what()) << endl;
    } catch (const exception& e) {
      cerr << "Uncaught exception while running game: \n" << colorize(Color::RED, e.what()) << endl;
      return 1;
    }
  }
  return 0;
}
