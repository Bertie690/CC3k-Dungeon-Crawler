module presetfloorgenerator;

#ifdef __INTELLISENSE__
#include <algorithm>
#include <fstream>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../entities/staircase.cc"
#include "../enums/direction.cc"
#include "../enums/gold-size.cc"
#include "../factories/dragon-hoard-factory.cc"
#include "../factories/gold-factory.cc"
#include "chamber.cc"
#include "floor.cc"
#include "position.cc"
#include "preset-floor-generator.cc"
#include "room.cc"
#else
import <algorithm>;
import <unordered_map>;
import <ranges>;
import <fstream>;
import <string>;
import <vector>;
import <memory>;
import <stdexcept>;
import <optional>;
import <set>;
import <utility>;
import direction;
import dragonhoardfactory;
import goldsize;
import goldfactory;
import chamber;
import position;
import floor;
import room;
import staircase;
#endif  // __INTELLISENSE__

void PerfectMatchHoardPlacementStrategy::placeDragonHoards(
    Floor& floor, const std::vector<Position>& hoardPositions,
    const std::vector<Position>& dragonPositions) {
  if (hoardPositions.size() < dragonPositions.size()) {
    throw std::invalid_argument{"Number of Dragon Hoards does not match number of Dragons!"};
  }

  std::set<Position> dragonPositionSet{dragonPositions.begin(), dragonPositions.end()};
  std::set<Position> unmatchedHoards;

  // handle all hoards that have no adjacent dragons (and collect the ones that do for the graph)
  for (const Position& hoardPosition : hoardPositions) {
    std::optional<Position> dragonPosition =
        findAdjacentHoard(floor, dragonPositionSet, hoardPosition);
    if (!dragonPosition.has_value()) {
      floor.getCell(hoardPosition).add(goldFactory.create(hoardPosition, GoldSize::DragonHoard));
    } else {
      unmatchedHoards.insert(hoardPosition);
    }
  }

  const std::optional<Result> matching =
      getPerfectMatching(floor, unmatchedHoards, dragonPositionSet);
  if (!matching.has_value()) {
    throw std::invalid_argument{
        "No perfect matching exists for the given Dragon Hoards and Dragon arrangement!"};
  }

  const Result& result = *matching;
  for (const auto& [dragonPosition, hoardPosition] : result.matches) {
    dragonHoardFactory.process(floor.getRoomAt(hoardPosition), hoardPosition, dragonPosition);
  }
  for (const Position& unmatchedHoard : result.unmatchedHoards) {
    floor.getCell(unmatchedHoard).add(goldFactory.create(unmatchedHoard, GoldSize::DragonHoard));
  }
}

std::optional<PerfectMatchHoardPlacementStrategy::Result>
PerfectMatchHoardPlacementStrategy::getPerfectMatching(const Floor& floor,
                                                       const std::set<Position>& hoardPositions,
                                                       const std::set<Position>& dragonPositions) {
  FlowNetwork network = buildFlowNetwork(floor, hoardPositions, dragonPositions);
  std::optional<Result> result = traverseFlowNetwork(network);
  return result;
}

void PerfectMatchHoardPlacementStrategy::FlowNetwork::addNodePair(const Position& hoardPos,
                                                                  const Position& dragonPos) {
  this->hoardNodes.emplace(hoardPos, Node{.pos = hoardPos, .isDragon = false});
  this->dragonNodes.emplace(dragonPos, Node{.pos = dragonPos, .isDragon = true});

  Node& hoardNode = this->hoardNodes[hoardPos];
  Node& dragonNode = this->dragonNodes[dragonPos];
  hoardNode.arcs.emplace_back(Arc{&hoardNode, &dragonNode});
  dragonNode.arcs.emplace_back(Arc{&dragonNode, &hoardNode});

  Arc* forwardArc = &hoardNode.arcs.back();
  Arc* reverseArc = &dragonNode.arcs.back();
  forwardArc->reverse = reverseArc;
  reverseArc->reverse = forwardArc;
}
std::size_t PerfectMatchHoardPlacementStrategy::FlowNetwork::size() const {
  return this->hoardNodes.size() + this->dragonNodes.size() + 2;  // +2 for source and sink
}

PerfectMatchHoardPlacementStrategy::FlowNetwork
PerfectMatchHoardPlacementStrategy::buildFlowNetwork(const Floor& floor,
                                                     const std::set<Position>& hoardPositions,
                                                     const std::set<Position>& dragonPositions) {
  FlowNetwork network;
  for (const Position& hoardPosition : hoardPositions) {
    for (const Direction dir : floor.getRoomAt(hoardPosition).getAdjacentCells(hoardPosition)) {
      Position candidate = hoardPosition + dir;
      if (dragonPositions.contains(candidate)) {
        network.addNodePair(hoardPosition, candidate);
      }
    }
  }

  network.sink = Node{.pos = Position{-1, -1}};
  network.source = Node{.pos = Position{-1, -1}};
  network.source.arcs.emplace_back(Arc{&network.source, &network.sink});
  network.sink.arcs.emplace_back(Arc{&network.sink, &network.source});
  {
    Arc* forwardSourceArc = &network.source.arcs.back();
    Arc* reverseSourceArc = &network.sink.arcs.back();
    forwardSourceArc->reverse = reverseSourceArc;
    reverseSourceArc->reverse = forwardSourceArc;
  }

  // Add an edge from source to the given node, and one from the given node to sink.
  const auto linkNode = [&network](Node& node) {
    network.source.arcs.emplace_back(Arc{&network.source, &node});
    node.arcs.emplace_back(Arc{&node, &network.source});
    {
      Arc* forwardSourceArc = &network.source.arcs.back();
      Arc* reverseSourceArc = &node.arcs.back();
      forwardSourceArc->reverse = reverseSourceArc;
      reverseSourceArc->reverse = forwardSourceArc;
    }

    node.arcs.emplace_back(Arc{&node, &network.sink});
    network.sink.arcs.emplace_back(Arc{&network.sink, &node});
    {
      Arc* forwardSinkArc = &node.arcs.back();
      Arc* reverseSinkArc = &network.sink.arcs.back();
      forwardSinkArc->reverse = reverseSinkArc;
      reverseSinkArc->reverse = forwardSinkArc;
    }
  };

  for (auto& [_, node] : network.hoardNodes) {
    linkNode(node);
  }
  for (auto& [_, node] : network.dragonNodes) {
    linkNode(node);
  }

  return network;
}

std::optional<PerfectMatchHoardPlacementStrategy::Result>
PerfectMatchHoardPlacementStrategy::traverseFlowNetwork(FlowNetwork& network) {
  static const int FLOW_CAP = 1;

  std::size_t maxFlow = 0;
  std::vector<Arc*> maximalPath;

  Node* s = &network.source;
  Node* t = &network.sink;

  while (true) {
    std::vector<Node*> queue{&network.source};
    std::unordered_map<Node*, Arc*> pred;
    pred.reserve(network.size());
    while (!queue.empty() && !pred.contains(t)) {
      Node* cur = queue.back();
      queue.pop_back();

      for (Arc& arc : cur->arcs) {
        if (!pred.contains(arc.to) && arc.to != s && FLOW_CAP > arc.flow) {
          pred.emplace(arc.to, &arc);
          queue.push_back(arc.to);
        }
      }
    }

    if (!pred.contains(t)) {
      // no augmenting path found, we are done
      break;
    }

    // store the augmenting path and update the flow along it
    maximalPath.clear();
    int deltaFlow = FLOW_CAP;
    for (Arc* cur = pred.at(t); cur != nullptr; cur = pred[cur->from]) {
      deltaFlow = std::min(deltaFlow, FLOW_CAP - cur->flow);
      maximalPath.push_back(cur);
    }

    for (Arc* cur = pred.at(t); cur != nullptr; cur = pred[cur->from]) {
      cur->flow += deltaFlow;
      cur->reverse->flow -= deltaFlow;
    }
    maxFlow += deltaFlow;
  }

  // check that the matching is perfect
  if (maxFlow != network.hoardNodes.size()) {
    return std::nullopt;
  }

  // we have our maximal flow, now we need to extract the matching from it
  Result result;
  for (const auto& [_, hoardNode] : network.hoardNodes) {
    bool included = false;
    for (const Arc& arc : hoardNode.arcs) {
      if (arc.flow > 0 && arc.to->isDragon) {
        result.matches.emplace(arc.to->pos, hoardNode.pos);
        included = true;
        break;
      }
    }
    if (!included) {
      result.unmatchedHoards.insert(hoardNode.pos);
    }
  }
  return result;
}

#pragma endregion  // Perfect Matching