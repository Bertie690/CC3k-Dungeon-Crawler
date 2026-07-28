export module factorybase;

#pragma once

#ifdef __INTELLISENSE__
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../utils/rng.cc"
#include "floor-region.cc"
#else
import <functional>;
import <memory>;
import <stdexcept>;
import <vector>;
import floorregion;
import rng;
#endif  // __INTELLISENSE__

export class FloorPopulationStrategy;
export class DummyFloorPopulationStrategy;
export class FactoryBase;

export std::shared_ptr<FloorPopulationStrategy> makeDefaultFloorPopulationStrategy();

// Base class for all entity factories.
// Operates on a per-Room basis, and is applied to a collection of `FloorRegions` by a `FloorPopulationStrategy`.
export class FactoryBase {
  // allow FloorPopulationStrategy to call processRoom without exposing it publicly
  friend class FloorPopulationStrategy;

 protected:
  RNG& rng;
  std::shared_ptr<FloorPopulationStrategy> strategy;

  // Process one FloorRegion.
  virtual void processRoom(FloorRegion& region) = 0;

 public:
  FactoryBase(RNG& rng, std::shared_ptr<FloorPopulationStrategy> strategy =
                            makeDefaultFloorPopulationStrategy())
      : rng{rng}, strategy{std::move(strategy)} {}
  virtual ~FactoryBase() = default;

  // Dynamically set this
  void setPopulationStrategy(std::shared_ptr<FloorPopulationStrategy> newStrategy) {
    if (!newStrategy) {
      throw std::invalid_argument{"FactoryBase::setPopulationStrategy cannot accept nullptr"};
    }
    strategy = std::move(newStrategy);
  }

  void execute(std::vector<FloorRegion>& regions);
};

// Strategy class determining how a FactoryBase is applied across a collection of FloorRegions,
// decoupling the execution pattern from the effects of the factory itself.
export class FloorPopulationStrategy {
 protected:
  // Protected helper to grant access to FactoryBase::processRoom without exposing it publicly.
  static void invoke(FactoryBase& factory, FloorRegion& region);

 public:
  virtual ~FloorPopulationStrategy() = default;
  // Apply the given factory across the specified regions according to this strategy's policy.
  virtual void populate(FactoryBase& factory, std::vector<FloorRegion>& regions) const = 0;
};

// Strategy to apply factory processing to randomly-chosen regions sampled with replacement.
export class RandomSampleFloorPopulationStrategy final : public FloorPopulationStrategy {
  RNG& rng;
  // The number of
  unsigned int count;

 public:
  RandomSampleFloorPopulationStrategy(RNG& rng, unsigned int count) : rng{rng}, count{count} {}
  virtual void populate(FactoryBase& factory, std::vector<FloorRegion>& regions) const override;
};

// Applies factory processing exactly once to every region.
export class SweepFloorPopulationStrategy final : public FloorPopulationStrategy {
 public:
  virtual void populate(FactoryBase& factory, std::vector<FloorRegion>& regions) const override;
};

// Strategy that wraps another `FloorPopulationStrategy`, restricting it to only the
// `FloorRegion`s for which the predicate returns `true`.
//
// Any `FloorRegion` that is filtered out will not have its availablePositions set updated,
// but will still be available for processing by other factories.
export class FilterFloorPopulationStrategy final : public FloorPopulationStrategy {
  // The wrapped strategy to apply to the filtered regions.
  std::shared_ptr<FloorPopulationStrategy> inner;
  // The predicate used to filter the regions.
  std::function<bool(const FloorRegion&)> predicate;

 public:
  FilterFloorPopulationStrategy(std::shared_ptr<FloorPopulationStrategy> inner,
                                std::function<bool(const FloorRegion&)> predicate)
      : inner{inner}, predicate{std::move(predicate)} {}
  virtual void populate(FactoryBase& factory, std::vector<FloorRegion>& regions) const override;
};

// Strategy used to indicate a Factory should not be allowed to execute across a collection of FloorRegions, and will throw if it is attempted.
//
// Used during preset floor generation and similar areas to prevent accidental use of random placement strategies.
export class DummyFloorPopulationStrategy final : public FloorPopulationStrategy {
 public:
  virtual void populate(FactoryBase& factory, std::vector<FloorRegion>& regions) const override;
};

#pragma region Implementation

void FloorPopulationStrategy::invoke(FactoryBase& factory, FloorRegion& region) {
  factory.processRoom(region);
}

void FactoryBase::execute(std::vector<FloorRegion>& regions) { strategy->populate(*this, regions); }

void RandomSampleFloorPopulationStrategy::populate(FactoryBase& factory,
                                                   std::vector<FloorRegion>& regions) const {
  for (unsigned int i = 0; i < count; ++i) {
    FloorRegion& region = regions[rng.intRange(regions.size())];
    invoke(factory, region);
  }
}

void SweepFloorPopulationStrategy::populate(FactoryBase& factory,
                                            std::vector<FloorRegion>& regions) const {
  for (FloorRegion& region : regions) {
    invoke(factory, region);
  }
}

void FilterFloorPopulationStrategy::populate(FactoryBase& factory,
                                             std::vector<FloorRegion>& regions) const {
  if (!inner) {
    throw std::logic_error{"FilterFloorPopulationStrategy is missing inner strategy"};
  }

  // NB: This is slightly more complicated due to the need to write back the mutated

  std::vector<FloorRegion*> eligibleRefs;
  std::vector<FloorRegion> eligible;
  for (FloorRegion& region : regions) {
    if (predicate(region)) {
      eligibleRefs.push_back(&region);
      eligible.push_back(FloorRegion{region.room, region.availablePositions});
    }
  }

  inner->populate(factory, eligible);

  // write back any positions that got mutated in the filtered version so later factories can use them
  for (std::size_t i = 0; i < eligible.size(); ++i) {
    eligibleRefs[i]->availablePositions = std::move(eligible[i].availablePositions);
  }
}

void DummyFloorPopulationStrategy::populate(FactoryBase&, std::vector<FloorRegion>&) const {
  throw std::logic_error{
      "DummyFloorPopulationStrategy::populate called. This factory is configured for "
      "deterministic usage and cannot execute floor-wide population."};
}

std::shared_ptr<FloorPopulationStrategy> makeDefaultFloorPopulationStrategy() {
  return std::make_shared<DummyFloorPopulationStrategy>();
}

#pragma endregion
