export module presetfloorgenerator;

#pragma once

#ifdef __INTELLISENSE__
#include <fstream>
#include <list>
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
import <list>;
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
  // Attempt to place dragon hoards on the given positions.
  // Throws if the hoards cannot be placed successfully.
  virtual void placeDragonHoards(Floor& floor, const std::vector<Position>& hoardPositions,
                                 const std::vector<Position>& dragonPositions) = 0;

 protected:
  const DragonHoardFactory& dragonHoardFactory;
  const GoldFactory& goldFactory;

 public:
  HoardPlacementStrategy(const DragonHoardFactory& dragonHoardFactory,
                         const GoldFactory& goldFactory)
      : dragonHoardFactory{dragonHoardFactory}, goldFactory{goldFactory} {}
};

class DefaultHoardPlacementStrategy final : public HoardPlacementStrategy {
 public:
  using HoardPlacementStrategy::HoardPlacementStrategy;

 private:
  virtual void placeDragonHoards(Floor& floor, const std::vector<Position>& hoardPositions,
                                 const std::vector<Position>& dragonPositions) override;
};
class PerfectMatchHoardPlacementStrategy final : public HoardPlacementStrategy {
 public:
  using HoardPlacementStrategy::HoardPlacementStrategy;

 private:
  // The result of a flow network traversal.
  struct Result {
    // A map matching each Dragon's Position to its corresponding Hoard's Position.
    std::map<Position, Position> matches;
    // A set of Hoard Positions that were not matched to any Dragon.
    std::set<Position> unmatchedHoards;
  };

  struct Arc;
  // A node in the flow network, representing either a Dragon or a Hoard.
  struct Node {
    // A list containing all forward arcs coming out of this node, alongside the respective backwards arcs.
    std::list<Arc> arcs;
    // The position of the hoard or dragon represented by this node.
    Position pos;
    // Whether this Node represents a Dragon or a Hoard.
    bool isDragon = false;
  };
  // An arc in the flow network, representing an adjacency relationship between a Dragon and its Hoard.
  struct Arc {
    Node* from;
    Node* to;
    int flow = 0;
    Arc* reverse = nullptr;
  };
  // Struct representing the flow network of Dragons and Hoards, with a source and sink node.
  struct FlowNetwork {
    // A map of all hoard nodes in the flow network, keyed by their Position.
    std::map<Position, Node> hoardNodes;
    // A map of all dragon nodes in the flow network, keyed by their Position.
    std::map<Position, Node> dragonNodes;
    // The source node of the flow network, representing the starting point for flow traversal.
    Node source;
    // The sink node of the flow network, representing the endpoint for flow traversal.
    Node sink;

    // Add a pair of nodes to the flow network, representing a Dragon and its adjacent Hoard.
    void addNodePair(const Position& hoardPos, const Position& dragonPos);
    // Return the number of nodes in the flow network.
    size_t size() const;
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
  const EnemyFactory enemyFactory;
  const GoldFactory goldFactory;
  const PotionFactory potionFactory;
  const DragonHoardFactory dragonHoardFactory;
  std::unique_ptr<HoardPlacementStrategy> strategy;

  std::ifstream input;
  unsigned int nextFloor;

  void placePresetEntities(Floor& floor, const std::vector<std::string>& floorLines);

 public:
  PresetFloorGenerator(RNG& rng, const std::string& fileName, unsigned int floorNumber = 0,
                       bool useImprovedHoardResolution = false);
  virtual Floor generateFloor() override;
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
