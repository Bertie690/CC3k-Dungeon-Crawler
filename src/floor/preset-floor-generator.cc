export module presetfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include <fstream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "../factories/dragon-hoard-factory.cc"
#include "../factories/enemy-factory.cc"
#include "../factories/gold-factory.cc"
#include "../factories/potion-factory.cc"
#include "../floor/position.cc"
#include "../utils/rng.cc"
#include "floor-generator.cc"
#include "floor.cc"
#else
import <fstream>;
import <string>;
import <optional>;
import <vector>;
import <map>;
import <set>;
import <utility>;
import <memory>;
import dragonhoardfactory;
import enemyfactory;
import goldfactory;
import potionfactory;
import rng;
import position;
import floor;
import floorgenerator;
#endif  // __INTELLISENSE__

export class PresetFloorGenerator;

class HoardPlacementStrategy {
  friend class PresetFloorGenerator;

  HoardPlacementStrategy(DragonHoardFactory& dragonHoardFactory, GoldFactory& goldFactory)
      : dragonHoardFactory{dragonHoardFactory}, goldFactory{goldFactory} {}

 protected:
  DragonHoardFactory& dragonHoardFactory;
  GoldFactory& goldFactory;

  // Attempt to place dragon hoards on the given positions.
  // Throws if the hoards cannot be placed successfully.
  virtual void placeDragonHoards(Floor& floor, const std::vector<Position>& hoardPositions,
                                 const std::vector<Position>& dragonPositions) = 0;
};

class DefaultHoardPlacementStrategy final : public HoardPlacementStrategy {
  virtual void placeDragonHoards(Floor& floor, const std::vector<Position>& hoardPositions,
                                 const std::vector<Position>& dragonPositions) override;
};
class PerfectMatchHoardPlacementStrategy final : public HoardPlacementStrategy {
  struct Result {
    std::map<Position, Position> matches;
    std::set<Position> unmatchedHoards;
  };

  struct Arc;
  struct Node {
    // Array containing both the forward and reverse edges.
    std::vector<Arc> arcs;
    Position pos;
    // marker to avoid needing to re-check every node
    bool isDragon = false;
  };
  struct Arc {
    Node* from;
    Node* to;
    int flow = 0;
    Arc* reverse = nullptr;
  };
  struct FlowNetwork {
    std::map<Position, Node> hoardNodes;
    std::map<Position, Node> dragonNodes;
    Node source;
    Node sink;

    void addNodePair(const Position& hoardPos, const Position& dragonPos);
    size_t size() const;
    std::vector<Arc*> getPath(Node* start, Node* end);
  };

  // Convert the given input positions into a flow network to be traversed.
  FlowNetwork buildFlowNetwork(const Floor& floor, const std::set<Position>& hoardPositions,
                               const std::set<Position>& dragonPositions);

  // Traverse the flow network using Edmonds-Karp Algorithm.
  std::optional<Result> traverseFlowNetwork(FlowNetwork& network);

  // Obtain the perfect matching for the given bipartite graph of hoardPositions and dragonPositions, if one exists.
  std::optional<Result> getPerfectMatching(const Floor& floor,
                                           const std::set<Position>& hoardPositions,
                                           const std::set<Position>& dragonPositions);

  virtual void placeDragonHoards(Floor& floor, const std::vector<Position>& hoardPositions,
                                 const std::vector<Position>& dragonPositions) override;
};

export class PresetFloorGenerator : public FloorGenerator {
  RNG& rng;
  EnemyFactory enemyFactory;
  GoldFactory goldFactory;
  PotionFactory potionFactory;
  DragonHoardFactory dragonHoardFactory;
  std::unique_ptr<HoardPlacementStrategy> strategy;

  std::ifstream input;
  int nextFloor = 1;

  void placePresetEntities(Floor& floor, const std::vector<std::string>& floorLines);

 public:
  // Generates Floor using file input provided entity placements
  PresetFloorGenerator(RNG& rng, const std::string& fileName,
                       bool allowAmbiguousDragonPlacement = false);
  Floor generateFloor() override;
};

// private namespace for various hardcoded symbols used in preset floor generation
namespace PresetSymbols {
  const char STAIRCASE_SYMBOL = '\\';
  const char PLAYER_SPAWN_SYMBOL = '@';

  const char HUMAN_SYMBOL = 'H';
  const char DWARF_SYMBOL = 'W';
  const char ELF_SYMBOL = 'E';
  const char ORC_SYMBOL = 'O';
  const char MERCHANT_SYMBOL = 'M';
  const char HALFLING_SYMBOL = 'L';
  const char DRAGON_SYMBOL = 'D';

  const char RH_POTION_SYMBOL = '0';
  const char BA_POTION_SYMBOL = '1';
  const char BD_POTION_SYMBOL = '2';
  const char PH_POTION_SYMBOL = '3';
  const char WA_POTION_SYMBOL = '4';
  const char WD_POTION_SYMBOL = '5';

  const char NORMAL_GOLD_SYMBOL = '6';
  const char SMALL_HOARD_SYMBOL = '7';
  const char MERCHANT_HOARD_SYMBOL = '8';
  const char DRAGON_HOARD_SYMBOL = '9';
}  // namespace PresetSymbols

std::optional<Position> findAdjacentHoard(const Floor& floor,
                                          const std::set<Position>& hoardPositions,
                                          const Position& dragonPosition);
