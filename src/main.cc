#ifdef __INTELLISENSE__
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "display/tui-renderer.cc"
#include "display/renderer.cc"
#include "enums/action.cc"
#include "enums/direction.cc"
#include "enums/race-type.cc"
#include "game/game.cc"
#include "input/stdin-input-handler.cc"
#include "input/input-handler.cc"
#else
import <iostream>;
import <memory>;
import <string>;
import <vector>;
import <fstream>;
import <cstdlib>;
import tuirenderer;
import renderer;
import action;
import direction;
import racetype;
import game;
import stdininputhandler;
import inputhandler;
#endif  // __INTELLISENSE__

using namespace std;

struct FeatureFlags {  // TODO : Implement feature flags in the game logic
  bool enableDebugMode = false;
  bool enableSlowMode = false;
  bool enableEnhancedGraphics = false;
  bool enableSoundEffects = false;
};

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

FeatureFlags parseFeatureFlags(int argc, char* argv[]) {
  FeatureFlags flags;

  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "--debug") {
      flags.enableDebugMode = true;
    } else if (arg == "--slow") {
      flags.enableSlowMode = true;
    } else if (arg == "--enhanced-graphics") {
      flags.enableEnhancedGraphics = true;
    } else if (arg == "--sound") {
      flags.enableSoundEffects = true;
    }
  }
  return flags;
}

char convertNumberToItem(int number) {
  switch (number) {
    case 0:
      return 'R';  // RH (Restoration Health)
    case 1:
      return 'B';  // BA (Boost Attack)
    case 2:
      return 'D';  // BD (Boost Defense)
    case 3:
      return 'P';  // PH (Poison Health)
    case 4:
      return 'W';  // WA (Wound Attack)
    case 5:
      return 'V';  // WD (Wound Defense)
    case 6:
      return 'G';  // normal gold pile
    case 7:
      return 'S';  // small hoard
    case 8:
      return 'M';  // merchant hoard
    case 9:
      return 'D';  // dragon hoard
    default:
      return '.';  // unknown item
  }
}

int main(int argc, char* argv[]) {
  string floorFile = parseFloorFile(argc, argv);
  unsigned int seed = parseSeed(argc, argv);
  FeatureFlags flags = parseFeatureFlags(argc, argv);

  if (flags.enableDebugMode) {
    cout << "Debug mode enabled" << endl;
  }

  if (flags.enableSlowMode) {
    cout << "Slow mode enabled" << endl;
  }

  if (flags.enableEnhancedGraphics) {
    cout << "Enhanced graphics enabled" << endl;
  }

  if (flags.enableSoundEffects) {
    cout << "Sound effects enabled" << endl;
  }

  unique_ptr<InputHandler> inputHandler = make_unique<StdinInputHandler>();
  unique_ptr<Renderer> renderer = make_unique<TUIRenderer>();

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
  while (true) {
    try {
      inputHandler->processInput();
    } catch (const exception& e) {
      cerr << "Input error: " << e.what() << endl;
    }
  }
  return 0;
}
